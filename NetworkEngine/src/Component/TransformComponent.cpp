#include "Component/TransformComponent.h"
#include "Component/RigidbodyComponent.h"
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include "Scene.h"
#include "Entity.h"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>


#include "Component/CameraComponent.h"
#include "Functions/Crc32.h"
#include "Functions/Serialization.h"

namespace GDE
{
    TransformComponent::~TransformComponent()
    {
        if(_transform->parent() == &Scene::getMagnumScene())
            delete _transform;
    }

    void TransformComponent::interpolate(const Timing& timing)
    {
        if (_updateInterpolation)
        {
            if(_interpolationPositionsRef[0].second != 0 && _interpolationPositionsRef[1].second != 0)
            {
                _updateInterpolation = false;
                _dPos = _interpolationPositionsRef[1].first - _interpolationPositionsRef[0].first;
                _dPos /= (_interpolationPositionsRef[1].second - _interpolationPositionsRef[0].second);

                const glm::vec3 currentEuler = glm::eulerAngles(glm::normalize(_interpolationRotationRef[1].first));
                const glm::vec3 oldEuler = glm::eulerAngles(glm::normalize(_interpolationRotationRef[0].first));

                _dRot = currentEuler - oldEuler;
                _dRot /= (_interpolationRotationRef[1].second - _interpolationRotationRef[0].second);
                //_dRot *= 0.85;
            }
        }

        Magnum::Vector3 currentPosition = _transform->transformation().translation();

        const Magnum::Matrix3 rotationScale = _transform->transformation().rotationScaling();
        // Convert to GLM
        glm::mat3 glmRotationScale = glm::mat3(rotationScale);

        // Normalize each column to remove scale
        glmRotationScale[0] = glm::normalize(glmRotationScale[0]);
        glmRotationScale[1] = glm::normalize(glmRotationScale[1]);
        glmRotationScale[2] = glm::normalize(glmRotationScale[2]);

        // Convert to a quaternion
        glm::quat currentRotation = glm::quat_cast(glmRotationScale);

        float dt = timing._dt / _lastFrameDuration;

        const glm::vec3 currentEuler = glm::eulerAngles(glm::normalize(currentRotation));
        //const glm::vec3 oldEuler = glm::eulerAngles(glm::normalize(_interpolationOldRotations[0]));

        glm::quat rot(currentEuler +_dRot);

        setTransformation(rot, currentPosition + _dPos);

        _lastFrameDuration = timing._dt;
    }

    void TransformComponent::setup(const ComponentDescription& init_value)
    {
        _transform = new Object3D();
        //_transform.setParent(&Scene::getMagnumScene());

        if (init_value.parameters.contains("scale"))
        {
            _transform->scale({ init_value.parameters.at("scale")[0].as<float>(),
                 init_value.parameters.at("scale")[1].as<float>(),
                 init_value.parameters.at("scale")[2].as<float>() }
            );
        }

        if (init_value.parameters.contains("rotation"))
        {
            _rotationVector.x() = init_value.parameters.at("rotation")[0].as<float>();
            _rotationVector.y() = init_value.parameters.at("rotation")[1].as<float>();

            {
                float x = glm::radians(init_value.parameters.at("rotation")[0].as<float>());
                float y = glm::radians(init_value.parameters.at("rotation")[1].as<float>());
                float z = glm::radians(init_value.parameters.at("rotation")[2].as<float>());

                // Create quaternion from Euler angles (Z * Y * X order, which is common)
                Magnum::Quaternion q = Magnum::Quaternion::rotation(Magnum::Rad(z), Magnum::Vector3::zAxis()) *
                    Magnum::Quaternion::rotation(Magnum::Rad(y), Magnum::Vector3::yAxis()) *
                    Magnum::Quaternion::rotation(Magnum::Rad(x), Magnum::Vector3::xAxis());

                _transform->rotate(q);
            }

        }

        if (init_value.parameters.contains("position"))
        {
            _transform->translate({ init_value.parameters.at("position")[0].as<float>(),
                init_value.parameters.at("position")[1].as<float>(),
                init_value.parameters.at("position")[2].as<float>() }
            );
        }
        _serializationOldPosition = _transform->transformation().translation();

        Magnum::Matrix3 rotationScale = _transform->transformation().rotationScaling();
        // Convert to GLM
        glm::mat3 glmRotationScale = glm::mat3(rotationScale);

        // Normalize each column to remove scale
        glmRotationScale[0] = glm::normalize(glmRotationScale[0]);
        glmRotationScale[1] = glm::normalize(glmRotationScale[1]);
        glmRotationScale[2] = glm::normalize(glmRotationScale[2]);

        // Convert to a quaternion
        _serializationOldRotation = glm::quat_cast(glmRotationScale);     

        _interpolationPositionsRef[0] = { Magnum::Vector3(), 0 };
        _interpolationPositionsRef[1] = { Magnum::Vector3(), 0 };
    }
    void TransformComponent::resolve()
    {
        if (auto transform = owner().getParent()->getComponent<TransformComponent>())
        {
            _transform->setParent(transform->_transform);
        }
        else
        {
            _transform->setParent(&Scene::getMagnumScene());
        }
    }
    void TransformComponent::setValue(std::string_view variable, float value)
    {
        switch (FIELD_MAP.at(std::string(variable)))
        {
        case GDE::TransformComponent::P_x:
            _transform->translate(Magnum::Vector3(-_transform->transformation().translation().x() + value, 0, 0));
            break;
        case GDE::TransformComponent::P_y:
            _transform->translate(Magnum::Vector3(0, -_transform->transformation().translation().y() + value, 0));
            break;
        case GDE::TransformComponent::P_z:
            _transform->translate(Magnum::Vector3(0, 0, -_transform->transformation().translation().z() + value));
            break;
        case GDE::TransformComponent::R_x:
        {
            _rotationVector.x() = value;
            Magnum::Matrix3 rotationScale = _transform->transformation().rotationScaling();

            // Convert to GLM
            glm::mat3 glmRotationScale = glm::mat3(rotationScale);

            // Normalize each column to remove scale
            glmRotationScale[0] = glm::normalize(glmRotationScale[0]);
            glmRotationScale[1] = glm::normalize(glmRotationScale[1]);
            glmRotationScale[2] = glm::normalize(glmRotationScale[2]);

            // Convert to a quaternion
            glm::quat rotation = glm::quat_cast(glmRotationScale);
            glm::vec3 eulerAngles = glm::eulerAngles(rotation);
            
            _transform->rotateXLocal(Magnum::Rad(-eulerAngles.x));
            _transform->rotateXLocal(Magnum::Rad(glm::radians(value)));
        }
            break;
        case GDE::TransformComponent::R_y:
        {
            _rotationVector.y() = value;
            Magnum::Matrix3 rotationScale = _transform->transformation().rotationScaling();

            // Convert to GLM
            glm::mat3 glmRotationScale = glm::mat3(rotationScale);

            // Normalize each column to remove scale
            glmRotationScale[0] = glm::normalize(glmRotationScale[0]);
            glmRotationScale[1] = glm::normalize(glmRotationScale[1]);
            glmRotationScale[2] = glm::normalize(glmRotationScale[2]);

            // Convert to a quaternion
            glm::quat rotation = glm::quat_cast(glmRotationScale);
            glm::vec3 eulerAngles = glm::eulerAngles(rotation);

            _transform->rotateYLocal(Magnum::Rad(-eulerAngles.y));
            _transform->rotateYLocal(Magnum::Rad(glm::radians(value)));
        }
            break;
        case GDE::TransformComponent::R_z:
        {
            Magnum::Matrix3 rotationScale = _transform->transformation().rotationScaling();

            // Convert to GLM
            glm::mat3 glmRotationScale = glm::mat3(rotationScale);

            // Normalize each column to remove scale
            glmRotationScale[0] = glm::normalize(glmRotationScale[0]);
            glmRotationScale[1] = glm::normalize(glmRotationScale[1]);
            glmRotationScale[2] = glm::normalize(glmRotationScale[2]);

            // Convert to a quaternion
            glm::quat rotation = glm::quat_cast(glmRotationScale);
            glm::vec3 eulerAngles = glm::eulerAngles(rotation);

            _transform->rotateZLocal(Magnum::Rad(-eulerAngles.z));
            _transform->rotateZLocal(Magnum::Rad(glm::radians(value)));
        }
            break;
        case GDE::TransformComponent::S_x:
        {
            Magnum::Vector3 scale = _transform->transformation().scaling();
            _transform->scale(Magnum::Vector3(value / scale.x(), 1, 1));
        }
            break;
        case GDE::TransformComponent::S_y:
        {
            Magnum::Vector3 scale = _transform->transformation().scaling();
            _transform->scale(Magnum::Vector3(1, value / scale.y(), 1));
        }
            break;
        case GDE::TransformComponent::S_z:
        {
            Magnum::Vector3 scale = _transform->transformation().scaling();
            _transform->scale(Magnum::Vector3(1, 1, value / scale.z()));
        }
            break;
        }

        if (auto rigidBody = owner().getComponent<GDE::RigidbodyComponent>())
        {
            rigidBody->syncPose();
        }
    }
    std::string TransformComponent::serialize()
    {
        constexpr uint32_t netId = Crc32::crc32(type);
        const uint32_t netIdCpy = netId;
        std::string outData;

        outData.resize(sizeof(netIdCpy) + ((Serialization::BIT_PER_FLOAT * 3 / 32) + 1) * 4 /*byte for position*/ + sizeof(Serialization::serialized_quaternion));
        memcpy(outData.data(), &netIdCpy, sizeof(netIdCpy));
        const Magnum::Vector3& position = _transform->transformation().translation();        

        Magnum::Matrix3 rotationScale = _transform->transformation().rotationScaling();
        // Convert to GLM
        glm::mat3 glmRotationScale = glm::mat3(rotationScale);

        // Normalize each column to remove scale
        glmRotationScale[0] = glm::normalize(glmRotationScale[0]);
        glmRotationScale[1] = glm::normalize(glmRotationScale[1]);
        glmRotationScale[2] = glm::normalize(glmRotationScale[2]);

        // Convert to a quaternion
        glm::quat rotation = glm::normalize(glm::quat_cast(glmRotationScale));

        if (position == _serializationOldPosition && rotation == _serializationOldRotation)
        {
            sameValueCounter++;
        }
        else
        {
            sameValueCounter = 0;
        }
        if(sameValueCounter < 60)
        {
            _serializationOldPosition = position;
            _serializationOldRotation = rotation;

            Serialization::serialized_float xy = Serialization::combineFloat(Serialization::serializeFloat(position.x()), Serialization::serializeFloat(position.y()));
            Serialization::serialized_float z = Serialization::serializeFloat(position.z());

            Serialization::serialized_quaternion quatValue = Serialization::serializeQuaternion(rotation);
            memcpy(outData.data() + sizeof(netIdCpy), &xy, sizeof(xy));
            memcpy(outData.data() + sizeof(netIdCpy) + sizeof(xy), &z, sizeof(z));
            memcpy(outData.data() + sizeof(netIdCpy) + sizeof(xy) + sizeof(z), &quatValue, sizeof(quatValue));

            return outData;
        }
        return "";
    }
    void TransformComponent::deserialize(char*& data, uint32_t frameIndex)
    {
        constexpr int POSITION_BEGIN_BYTE = sizeof(uint32_t);
        constexpr int ROTATION_BEGIN_BYTE = POSITION_BEGIN_BYTE + 2 * sizeof(uint32_t);
        Serialization::serialized_float xy;
        Serialization::serialized_float z;
        Serialization::serialized_quaternion rotation;
        if(frameIndex > lastSnapshotIndex || frameIndex == 0)
        {
            lastSnapshotIndex = frameIndex;

            data += POSITION_BEGIN_BYTE;
            memcpy(&xy, data, sizeof(xy));
            data += sizeof(xy);
            memcpy(&z, data, sizeof(z));
            data += sizeof(z);
            memcpy(&rotation, data, sizeof(rotation));
            data += sizeof(rotation);

            auto [x, y] = Serialization::separateFloat(xy);
            glm::quat quat = Serialization::deserializeQuaternion(rotation);

            Magnum::Vector3 pos(Serialization::deserializeFloat(x), Serialization::deserializeFloat(y), Serialization::deserializeFloat(z));

            setTransformation(quat, pos);
            //_interpolationOldPositions[0] = _interpolationOldPositions[1];
            //_interpolationOldPositions[1] = pos;
            //_interpolationOldRotations[0] = _interpolationOldRotations[1];
            //_interpolationOldRotations[1] = quat;

            _interpolationPositionsRef[0] = _interpolationPositionsRef[1];
            _interpolationPositionsRef[1] = { pos, frameIndex };
            
            _interpolationRotationRef[0] = _interpolationRotationRef[1];
            _interpolationRotationRef[1] = { quat, frameIndex };


            _updateInterpolation = true;
        }
        else
        {
            data += POSITION_BEGIN_BYTE;
            data += sizeof(xy);
            data += sizeof(z);
            data += sizeof(rotation);
        }
    }
    void GDE::TransformComponent::setTransformation(const glm::quat& quat, const Magnum::Vector3& pos)
    {
        glm::mat3 glmRotationScale = glm::mat3(_transform->transformation().rotationScaling());
        glm::vec3 scale;
        scale.x = glm::length(glm::vec3(glmRotationScale[0]));
        scale.y = glm::length(glm::vec3(glmRotationScale[1]));
        scale.z = glm::length(glm::vec3(glmRotationScale[2]));

        // Create a scaled rotation matrix from the quaternion
        Magnum::Matrix4 newRotationWithScale = Magnum::Matrix4::from(Magnum::Quaternion({ quat.x, quat.y, quat.z }, quat.w).toMatrix(), pos) * Magnum::Matrix4::scaling(Magnum::Vector3(scale.x, scale.y, scale.z));

        _transform->setTransformation(newRotationWithScale);
    }
}

#include "Component/TransformComponent.h"
#include "Component/RigidbodyComponent.h"
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include "Scene.h"
#include "Entity.h"
#include <Magnum/Math/Matrix3.h>
#include <Magnum/GlmIntegration/Integration.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

#include "Component/CameraComponent.h"

namespace GDE
{
    TransformComponent::~TransformComponent()
    {
        if(_transform->parent() == &Scene::getMagnumScene())
            delete _transform;
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
            rotationVector.x() = init_value.parameters.at("rotation")[0].as<float>();
            rotationVector.y() = init_value.parameters.at("rotation")[1].as<float>();

            if (owner().getComponent<CameraComponent>() == nullptr) {
                _transform->rotateX(Magnum::Rad(glm::radians(init_value.parameters.at("rotation")[0].as<float>())));
                _transform->rotateY(Magnum::Rad(glm::radians(init_value.parameters.at("rotation")[1].as<float>())));
                _transform->rotateZ(Magnum::Rad(glm::radians(init_value.parameters.at("rotation")[2].as<float>())));
            }

        }

        if (init_value.parameters.contains("position"))
        {
            _transform->translate({ init_value.parameters.at("position")[0].as<float>(),
                init_value.parameters.at("position")[1].as<float>(),
                init_value.parameters.at("position")[2].as<float>() }
            );
        }
        
        
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
            rotationVector.x() = value;
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
            rotationVector.y() = value;
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
}

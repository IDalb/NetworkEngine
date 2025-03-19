#include "Component\TransformComponent.h"

#include "Entity.h"

namespace GDE
{
    void TransformComponent::setup(const ComponentDescription &init_value) {
       /* const auto& params = init_value.parameters;

        _position.x() = params.contains("position") ? params.at("position")["x"].as<float>() : 0.0f;
        _position.y() = params.contains("position/y") ? params.at("position/y").as<float>() : 0.0f;
        _position.z() = params.contains("position/z") ? params.at("position/z").as<float>() : 0.0f;

        _rotation.x() = params.contains("rotation/x") ? params.at("rotation/x").as<float>() : 0.0f;
        _rotation.y() = params.contains("rotation/y") ? params.at("rotation/y").as<float>() : 0.0f;
        _rotation.z() = params.contains("rotation/z") ? params.at("rotation/z").as<float>() : 0.0f;

        _scale.x() = params.contains("scale/x") ? params.at("scale/x").as<float>() : 1.0f;
        _scale.y() = params.contains("scale/y") ? params.at("scale/y").as<float>() : 1.0f;
        _scale.z() = params.contains("scale/z") ? params.at("scale/z").as<float>() : 1.0f;*/
    }

    void TransformComponent::resolve() {
        //for (const auto& child : owner().getChildren()) {
        //    if (const auto transform = child->getComponent<TransformComponent>()) {
        //        Vector3 worldPosition = transform->getPosition();
        //        transform->setPosition(worldPosition);
        //
        //        Vector3 worldRotation = transform->getRotation();
        //        transform->setRotation(worldRotation);
        //
        //        Vector3 worldScale = transform->getScale();
        //        transform->setScale(worldScale);
        //    }
        }
    }

    // ===== Position ===== //
    void TransformComponent::setPosition(float x, float y, float z) {
        for (const auto& child : owner().getChildren()) {
            if (const auto transform = child->getComponent<TransformComponent>()) {
                Vector3 relativePosition = transform->getRelativePosition();
                transform->setPosition(x + relativePosition.x(), y + relativePosition.y(), z + relativePosition.z());
            }
        }

        _position.x() = x;
        _position.y() = y;
        _position.z() = z;
    }

    void TransformComponent::translate(float x, float y, float z) {
        for (const auto& child : owner().getChildren()) {
            if (const auto transform = child->getComponent<TransformComponent>()) {
                transform->translate(x, y, z);
            }
        }

        _position.x() += x;
        _position.y() += y;
        _position.z() += z;
    }

    Vector3 TransformComponent::getRelativePosition() const {
        const EntityRef parent = owner().getParent();
        if (parent == nullptr)
            return _position;

        const auto* parentTransform = parent->getComponent<TransformComponent>();
        if (parentTransform == nullptr)
            return _position;

        return _position - parentTransform->_position;
    }


    void TransformComponent::setRelativePosition(float x, float y, float z) {
        const EntityRef parent = owner().getParent();
        if (parent == nullptr) {
            setPosition(x, y, z);
            return;
        }

        const auto* parentTransform = parent->getComponent<TransformComponent>();
        if (parentTransform == nullptr) {
            setPosition(x, y, z);
            return;
        }

        setPosition(parentTransform->_position.x() + x, parentTransform->_position.y() + y, parentTransform->_position.z() + z);
    }


    // ===== Rotation ===== //
    void TransformComponent::setRotation(float x, float y, float z) {
        for (const auto& child : owner().getChildren()) {
            if (const auto transform = child->getComponent<TransformComponent>()) {
                Vector3 relativeRotation = transform->getRelativeRotation();
                transform->setRotation(x + relativeRotation.x(), y + relativeRotation.y(), z + relativeRotation.z());
            }
        }

        _rotation.x() = x;
        _rotation.y() = y;
        _rotation.z() = z;
    }

    void TransformComponent::rotate(float x, float y, float z) {
        for (const auto& child : owner().getChildren()) {
            if (const auto transform = child->getComponent<TransformComponent>()) {
                transform->rotate(x, y, z);
            }
        }

        _rotation.x() += x;
        _rotation.y() += y;
        _rotation.z() += z;
    }

    Vector3 TransformComponent::getRelativeRotation() const {
        const EntityRef parent = owner().getParent();
        if (parent == nullptr)
            return _rotation;

        const auto* parentTransform = parent->getComponent<TransformComponent>();
        if (parentTransform == nullptr)
            return _rotation;

        return _rotation - parentTransform->_rotation;
    }


    void TransformComponent::setRelativeRotation(float x, float y, float z) {
        const EntityRef parent = owner().getParent();
        if (parent == nullptr) {
            setRotation(x, y, z);
            return;
        }

        const auto* parentTransform = parent->getComponent<TransformComponent>();
        if (parentTransform == nullptr) {
            setRotation(x, y, z);
            return;
        }

        setRotation(parentTransform->_rotation.x() + x, parentTransform->_rotation.y() + y, parentTransform->_rotation.z() + z);
    }


    // ===== Scale ===== //
    void TransformComponent::setScale(float x, float y, float z) {
        for (const auto& child : owner().getChildren()) {
            if (const auto transform = child->getComponent<TransformComponent>()) {
                Vector3 relativeScale = transform->getRelativeScale();
                transform->setScale(x + relativeScale.x(), y + relativeScale.y(), z + relativeScale.z());
            }
        }

        _scale.x() = x;
        _scale.y() = y;
        _scale.z() = z;
    }

    Vector3 TransformComponent::getRelativeScale() const {
        const EntityRef parent = owner().getParent();
        if (parent == nullptr)
            return _scale;

        const auto* parentTransform = parent->getComponent<TransformComponent>();
        if (parentTransform == nullptr)
            return _scale;

        return _scale - parentTransform->_scale;
    }


    void TransformComponent::setRelativeScale(float x, float y, float z) {
        const EntityRef parent = owner().getParent();
        if (parent == nullptr) {
            setScale(x, y, z);
            return;
        }

        const auto* parentTransform = parent->getComponent<TransformComponent>();
        if (parentTransform == nullptr) {
            setScale(x, y, z);
            return;
        }

        setScale(parentTransform->_scale.x() + x, parentTransform->_scale.y() + y, parentTransform->_scale.z() + z);
    }
}

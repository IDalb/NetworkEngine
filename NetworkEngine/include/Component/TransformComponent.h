#pragma once

#include "Component.h"
#include "Magnum/Magnum.h"
#include "Magnum/Math/Vector3.h"

using namespace Magnum;

namespace GDE
{
    class TransformComponent : public Component
    {
    private:
        Vector3 _position{};
        Vector3 _rotation{};
        Vector3 _scale{};

    public:
        static constexpr auto type = "Transform";

        explicit TransformComponent(Entity& owner) : Component(owner) {}
        TransformComponent(const TransformComponent&) = default;
        TransformComponent& operator=(const TransformComponent&) = default;
        TransformComponent(TransformComponent&&) = default;
        TransformComponent& operator=(TransformComponent&&) = default;

        void setup(const ComponentDescription &init_value) override;
        void resolve() override;

        const Vector3& getPosition() const { return _position; }
        Vector3 getRelativePosition() const;
        void setPosition(const Vector3& new_position) { setPosition(new_position.x(), new_position.y(), new_position.z()); }
        void setPosition(float x, float y, float z);
        void setRelativePosition(const Vector3& new_position) { setRelativePosition(new_position.x(), new_position.y(), new_position.z()); }
        void setRelativePosition(float x, float y, float z);
        void translate(const Vector3& translation) { translate(translation.x(), translation.y(), translation.z()); }
        void translate(float x, float y, float z);

        const Vector3& getRotation() const { return _rotation; }
        Vector3 getRelativeRotation() const;
        void setRotation(const Vector3& new_rotation) { setRotation(new_rotation.x(), new_rotation.y(), new_rotation.z()); }
        void setRotation(float x, float y, float z);
        void setRelativeRotation(const Vector3& new_rotation) { setRelativeRotation(new_rotation.x(), new_rotation.y(), new_rotation.z()); }
        void setRelativeRotation(float x, float y, float z);
        void rotate(const Vector3& rotation) { rotate(rotation.x(), rotation.y(), rotation.z()); }
        void rotate(float x, float y, float z);

        const Vector3& getScale() const { return _scale; }
        Vector3 getRelativeScale() const;
        void setScale(const Vector3& new_scale) { setScale(new_scale.x(), new_scale.y(), new_scale.z()); }
        void setScale(float x, float y, float z);
        void setRelativeScale(const Vector3& new_scale) { setRelativeScale(new_scale.x(), new_scale.y(), new_scale.z()); }
        void setRelativeScale(float x, float y, float z);
    };
}

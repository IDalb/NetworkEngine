#pragma once

#include "Component.h"
#include "TypeDef.h"
namespace GDE
{
    class TransformComponent : public Component
    {
    private:
        Object3D _transform;

    public:
        static constexpr auto type = "Transform";

        explicit TransformComponent(Entity& owner) : Component(owner) {}
        TransformComponent(const TransformComponent&) = default;
        TransformComponent& operator=(const TransformComponent&) = default;
        TransformComponent(TransformComponent&&) = default;
        TransformComponent& operator=(TransformComponent&&) = default;

        void setup(const ComponentDescription &init_value) override;

        Object3D& getTransform() { return _transform; }
    };
}

#pragma once

#include "Component/Component.h"

namespace GDE
{
    class TransformComponent : public Component
    {
    private:
        Object3D* _transform;

        enum Field
        {
            P_x, P_y, P_z, R_x, R_y, R_z, S_x, S_y, S_z
        };

        inline static std::map<std::string, Field> FIELD_MAP = { {"position_x", P_x}, {"position_y", P_y}, {"position_z", P_z}
                                                              , {"rotation_x", R_x}, {"rotation_y", R_y}, {"rotation_z", R_z}
                                                              , {"scale_x", S_x}, {"scale_y", S_y}, {"scale_z", S_z} };

    public:
        static constexpr auto type = "Transform";

        explicit TransformComponent(Entity& owner) : Component(owner) {}
        TransformComponent(const TransformComponent&) = default;
        TransformComponent& operator=(const TransformComponent&) = default;
        TransformComponent(TransformComponent&&) = default;
        TransformComponent& operator=(TransformComponent&&) = default;
        ~TransformComponent();

        void setup(const ComponentDescription &init_value) override;
        void resolve();
        Object3D& getTransform() { return *_transform; }

        void setValue(std::string_view variable, float value) override;
    };
}

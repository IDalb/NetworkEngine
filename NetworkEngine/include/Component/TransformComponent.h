#pragma once

#include "Component/InterpolationComponent.h"
#include <Magnum/Math/Matrix3.h>
#include <Magnum/GlmIntegration/Integration.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
namespace GDE
{
    class TransformComponent : public InterpolationComponent
    {
    private:
        Object3D* _transform;
        glm::quat _serializationOldRotation{};
        Magnum::Vector3 _serializationOldPosition{};
        

        Magnum::Vector3 _dPos{};
        glm::vec3 _dRot{};
        std::array<std::pair<Magnum::Vector3, uint32_t>, 2> _interpolationPositionsRef{};
        std::array<std::pair<glm::quat, uint32_t>, 2> _interpolationRotationRef{};
        bool _updateInterpolation = true;
        

        float _lastFrameDuration = (1 / 60.f) * 1000;
        int sameValueCounter = 0;

        uint32_t lastSnapshotIndex = 0;
        enum Field
        {
            P_x, P_y, P_z, R_x, R_y, R_z, S_x, S_y, S_z
        };

        inline static std::map<std::string, Field> FIELD_MAP = { {"position_x", P_x}, {"position_y", P_y}, {"position_z", P_z}
                                                              , {"rotation_x", R_x}, {"rotation_y", R_y}, {"rotation_z", R_z}
                                                              , {"scale_x", S_x}, {"scale_y", S_y}, {"scale_z", S_z} };

    public:
        static constexpr auto type = "Transform";

        TransformComponent(Entity& owner) : Component(owner) {}
        TransformComponent(const TransformComponent&) = default;
        TransformComponent& operator=(const TransformComponent&) = default;
        TransformComponent(TransformComponent&&) = default;
        TransformComponent& operator=(TransformComponent&&) = default;
        ~TransformComponent();

        void interpolate(const Timing& timing) override;

        void setup(const ComponentDescription &init_value) override;
        void resolve();
        Object3D& getTransform() { return *_transform; }

        Magnum::Vector2 _rotationVector;

        void setValue(std::string_view variable, float value) override;

        std::string serialize() override;
        void deserialize(char*& data, uint32_t frameIndex) override;
        void setTransformation(const glm::quat& quat, const Magnum::Vector3& pos);
    };
}

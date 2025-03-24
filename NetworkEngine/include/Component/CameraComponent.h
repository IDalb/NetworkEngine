#pragma once
#include "Component.h"
#include <Magnum/GlmIntegration/Integration.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/Object.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>

#include "TransformComponent.h"
#include "Component/AlternateLogicComponent.h"
#include "Component/LogicComponent.h"
namespace GDE
{
    class CameraComponent : public AlternateLogicComponent, public LogicComponent
    {
    private:
        float cameraEditorTranslationSpeed = .25f;
        static constexpr float CAMERA_EDITOR_ROTATION_SPEED = .115f;

        Magnum::SceneGraph::Camera3D* _camera;
        float _projectionAngle = 35.f;
        float _aspectRatio = 1.f;
        float _farDistance = 0.001f;
        float _nearDistance = 100.f;

        Magnum::Deg _xAngle = Magnum::Deg(0.0f);
        Magnum::Deg _yAngle = Magnum::Deg(0.0f);

        Magnum::Matrix4 _CameraView;
        Magnum::Matrix4 _CameraViewBaseRotation;

        Magnum::Matrix4 ComputeCameraView(TransformComponent& transform);
        void update(const Timing& timing);
    public:
        static constexpr auto type = "Camera";

        CameraComponent(Entity& owner) : Component(owner) {}
        ~CameraComponent() = default;

        void updateAlternateLogic(const Timing& timing) override { update(timing); }
        void updateLogic(const Timing& timing) override { update(timing); }

        void setup(const ComponentDescription &init_value) override;
        void resolve() override;

        Magnum::SceneGraph::Camera3D* getCamera() { return _camera; };
    };
}


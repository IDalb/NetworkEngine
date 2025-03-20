#pragma once
#include "Component.h"
#include <Magnum/GlmIntegration/Integration.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/Object.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
namespace GDE
{
    class CameraComponent : public Component {
    private:
        Magnum::SceneGraph::Camera3D* _camera;
        float _projectionAngle = 35.f;
        float _aspectRatio = 1.f;
        float _farDistance = 0.001f;
        float _nearDistance = 100.f;

        Magnum::Matrix4 _CameraView;
    public:
        static constexpr auto type = "Camera";

        CameraComponent(Entity& owner) : Component(owner) {}
        ~CameraComponent() = default;

        void setup(const ComponentDescription &init_value) override;
        void resolve() override;

        Magnum::SceneGraph::Camera3D* getCamera() { return _camera; };
    };
}


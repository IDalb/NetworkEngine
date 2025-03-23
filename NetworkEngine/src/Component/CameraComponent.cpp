#include "Component/CameraComponent.h"

#include "Entity.h"
#include "../../../cmake-build-release/_deps/magnum-src/src/Magnum/Math/Quaternion.h"
#include "Component/TransformComponent.h"
#include "Magnum/GL/DefaultFramebuffer.h"
#include "System/InputSystem.h"


using namespace Magnum::Math::Literals;

namespace GDE
{
    float getValue(const ComponentDescription& init_value, const std::string& key, float default_value)
    {
        return init_value.parameters.contains(key) ? init_value.parameters.at(key).as<float>() : default_value;
    }

    void CameraComponent::update(const Timing& timing)
    {
        auto input = InputSystem::getInstance();

        // Rotation
        if (input.isMouseButtonHeld(GDE::Mouse::RIGHT)) {
            const Magnum::Vector2 mouseMvt = input.getMouseVelocity() * CAMERA_EDITOR_ROTATION_SPEED;
            _xAngle += static_cast<Magnum::Rad>(mouseMvt.y());
            _yAngle += static_cast<Magnum::Rad>(mouseMvt.x());

            _CameraView[0] = {1, 0, 0, 0};
            _CameraView[1] = {0, 1, 0, 0};
            _CameraView[2] = {0, 0, 1, 0};

            _CameraView = _CameraView
                * Magnum::Matrix4::rotationZ(-_yAngle)
                * Magnum::Matrix4::rotationX(-_xAngle);
        }

        // Translation
        const int xAxis = input.getAxis(Key::A, Key::D);
        const int yAxis = input.getAxis(Key::S, Key::W);
        const int zAxis = input.getAxis(Key::Q, Key::E);

        _CameraView[3] += CAMERA_EDITOR_TRANSLATION_SPEED * Magnum::Vector4(0, 0, static_cast<float>(zAxis), 0);
        _CameraView = _CameraView * Magnum::Matrix4::translation(Magnum::Vector3(static_cast<float>(xAxis), 0, -1.5f * static_cast<float>(yAxis)) * CAMERA_EDITOR_TRANSLATION_SPEED);

        owner().getComponent<TransformComponent>()->getTransform().setTransformation(_CameraView);
    }

    void CameraComponent::setup(const ComponentDescription &init_value) {
        _projectionAngle = getValue(init_value, "projection_angle", 45);
        _aspectRatio = getValue(init_value, "aspect_ratio", 1.0);
        _nearDistance = getValue(init_value, "near_distance", 0.001f);
        _farDistance = getValue(init_value, "far_distance", 100.0f);
    }
    void CameraComponent::resolve()
    {
        _camera = new Magnum::SceneGraph::Camera3D(owner().getComponent<TransformComponent>()->getTransform());

        _camera->setAspectRatioPolicy(Magnum::SceneGraph::AspectRatioPolicy::Extend);
        _camera->setProjectionMatrix(Magnum::Matrix4::perspectiveProjection(
            operator ""_degf(_projectionAngle),_aspectRatio, _nearDistance, _farDistance));
        _camera->setViewport(Magnum::GL::defaultFramebuffer.viewport().size());

        // Look at (0,0,0) with (0,0,1) as the up vector
        _CameraView = Magnum::Matrix4();
        _camera->cameraMatrix() = _CameraView;
        //_CameraView = Magnum::Matrix4::lookAt(Magnum::Vector3{ owner().getComponent<TransformComponent>()->getTransform().absoluteTransformation().translation() }, Magnum::Vector3{ 0.0f, 0.0f, 0.0f }, Magnum::Vector3{ 0.0f, 0.0f, 1.0f });
        //owner().getComponent<TransformComponent>()->getTransform().setTransformation(_CameraView);
    }
}

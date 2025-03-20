#include "Component/CameraComponent.h"

#include "Entity.h"
#include "Component/TransformComponent.h"
#include "Magnum/GL/DefaultFramebuffer.h"



using namespace Magnum::Math::Literals;

namespace GDE
{
    float getValue(const ComponentDescription& init_value, const std::string& key, float default_value)
    {
        return init_value.parameters.contains(key) ? init_value.parameters.at(key).as<float>() : default_value;
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
        _CameraView = Magnum::Matrix4::lookAt(Magnum::Vector3{ owner().getComponent<TransformComponent>()->getTransform().absoluteTransformation().translation() }, Magnum::Vector3{ 0.0f, 0.0f, 0.0f }, Magnum::Vector3{ 0.0f, 0.0f, 1.0f });
        owner().getComponent<TransformComponent>()->getTransform().setTransformation(_CameraView);
    }
}

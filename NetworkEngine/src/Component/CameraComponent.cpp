#include "Component/CameraComponent.h"

#include "Entity.h"
#include "Component/TransformComponent.h"
#include "Magnum/GL/DefaultFramebuffer.h"
#include "Magnum/SceneGraph/Camera.h"


using namespace Magnum::Math::Literals;

namespace GDE
{
    template<class T>
    T getValue(const ComponentDescription &init_value, const std::string &key, T default_value) {
        return init_value.parameters.contains(key) ? init_value.parameters.at(key).as<T>() : default_value;
    }

    void CameraComponent::setup(const ComponentDescription &init_value) {
        _camera = new Magnum::SceneGraph::Camera3D(owner().getComponent<TransformComponent>()->getTransform());

        _camera->setAspectRatioPolicy(Magnum::SceneGraph::AspectRatioPolicy::Extend);
        _camera->setProjectionMatrix(Magnum::Matrix4::perspectiveProjection(
            operator ""_degf(getValue(init_value, "projection_angle", 35.0)),
            getValue(init_value, "aspect_ratio", 1.0),
            getValue(init_value, "near_distance", 0.001f),
            getValue(init_value, "far_distance", 100.0f)
        ));
        _camera->setViewport(Magnum::GL::defaultFramebuffer.viewport().size());
    }
}

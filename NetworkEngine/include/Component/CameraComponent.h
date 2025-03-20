#pragma once
#include "Component.h"

namespace GDE
{
    class CameraComponent : public Component {
    private:
        Magnum::SceneGraph::Camera3D* _camera;

    public:
        static constexpr auto type = "Camera";

        CameraComponent(Entity& owner) : Component(owner) {}
        ~CameraComponent();

        void setup(const ComponentDescription &init_value) override;
        void resolve() override;

        Magnum::SceneGraph::Camera3D* getCamera() { return _camera; };
    };
}


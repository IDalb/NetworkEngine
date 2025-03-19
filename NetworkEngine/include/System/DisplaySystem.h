#pragma once
#include "System/System.h"
#include "Component/DisplayComponent.h"
#include "std.h"

#include <Magnum/Shaders/PhongGL.h>
#include <Magnum/SceneGraph/Drawable.h>

namespace GDE
{
    class DisplaySystem :
    public System
    {
    public:
        DisplaySystem() = default;
        DisplaySystem(const DisplaySystem&) = default;
        DisplaySystem(DisplaySystem&&) = default;
        DisplaySystem& operator=(const DisplaySystem&) = default;
        DisplaySystem& operator=(DisplaySystem&&) = default;

        ~DisplaySystem() = default;

        void setup();

        void iterate(const Timing& dt) override;

        void registerComponent(DisplayComponent* display_component);
        void removeComponent(DisplayComponent* display_component);

        static DisplaySystem& getInstance();

        Magnum::SceneGraph::DrawableGroup3D& getDrawable() { return _drawable; }
    private:
        std::unordered_set<DisplayComponent*> _displayComponents;
        Magnum::Shaders::PhongGL _shader{NoCreate};
        Magnum::SceneGraph::DrawableGroup3D _drawable;
    };
}


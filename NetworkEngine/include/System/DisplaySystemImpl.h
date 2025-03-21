#pragma once
#include "System/System.h"
#include "Component/DisplayComponent.h"
#include "std.h"

#include <Magnum/Shaders/PhongGL.h>
#include <Magnum/SceneGraph/Drawable.h>

namespace GDE
{
    class DisplaySystemImpl
    {
    public:
        DisplaySystemImpl() = default;
        DisplaySystemImpl(const DisplaySystemImpl&) = default;
        DisplaySystemImpl(DisplaySystemImpl&&) = default;
        DisplaySystemImpl& operator=(const DisplaySystemImpl&) = default;
        DisplaySystemImpl& operator=(DisplaySystemImpl&&) = default;

        ~DisplaySystemImpl() = default;

        void setup();
        void clean();

        virtual void iterate(const Timing& dt);

        void registerComponent(DisplayComponent* display_component);
        void removeComponent(DisplayComponent* display_component);

        Magnum::SceneGraph::DrawableGroup3D& getDrawable() { return _drawable; }
        void setEnable(bool enable) { _enable = enable; }
    protected:
        bool _enable = true;
        void renderUi();
        void renderScene(const GDE::Timing& dt);

        std::unordered_set<DisplayComponent*> _displayComponents;
        std::unique_ptr<Magnum::Shaders::PhongGL> _shader;
        Magnum::SceneGraph::DrawableGroup3D _drawable;
    };
}




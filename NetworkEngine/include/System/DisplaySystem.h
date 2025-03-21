#pragma once
#include "System/System.h"
#include "Component/DisplayComponent.h"
#include "std.h"

#include <Magnum/Shaders/PhongGL.h>
#include <Magnum/SceneGraph/Drawable.h>
#include "System/DisplaySystemImpl.h"
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

        void initImpl(std::function<std::unique_ptr<DisplaySystemImpl>()> implFun){ impl_ = implFun(); }

        void setup() { impl_->setup(); }
        void clean() { impl_->clean(); }

        void iterate(const Timing& dt) override { impl_->iterate(dt); }

        void registerComponent(DisplayComponent* display_component) { impl_->registerComponent(display_component); }
        void removeComponent(DisplayComponent* display_component) { impl_->removeComponent(display_component); }

        static DisplaySystem& getInstance()
        {
            static DisplaySystem display_system;
            return display_system;
        }

        void setEnable(bool enable) { impl_->setEnable(enable); }

        Magnum::SceneGraph::DrawableGroup3D& getDrawable() { return impl_->getDrawable(); }
    private:
        bool _enable = true;
        std::unique_ptr<DisplaySystemImpl> impl_;
    };
}


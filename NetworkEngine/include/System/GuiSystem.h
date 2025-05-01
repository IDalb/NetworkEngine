#pragma once
#include "System/System.h"
#include <Magnum/Math/Vector.h>
#include <Magnum/Platform/GlfwApplication.h>
#include "Component/GuiComponent.h"
#include "std.h"
#include <Magnum/ImGuiIntegration/Context.hpp>
namespace GDE
{
    class GuiSystem :
        public System
    {
    public:
        GuiSystem();
        GuiSystem(const GuiSystem&) = default;
        GuiSystem(GuiSystem&&) = default;
        GuiSystem& operator=(const GuiSystem&) = default;
        GuiSystem& operator=(GuiSystem&&) = default;

        ~GuiSystem() = default;

        void iterate(const Timing& dt) override;

        void registerComponent(GuiComponent* display_component);
        void removeComponent(GuiComponent* display_component);

        static GuiSystem& getInstance();
        inline static bool _exist = false;

        void setup(const Magnum::Vector2& size, const Magnum::Vector2i& windowSize, const Magnum::Vector2i& framebufferSize, Magnum::Platform::GlfwApplication* app);
        void relayout(const Magnum::Vector2& size, const Magnum::Vector2i& windowSize, const Magnum::Vector2i& framebufferSize);

        void remove();

        bool keyPressEvent(Magnum::Platform::GlfwApplication::KeyEvent& event);
        bool keyReleaseEvent(Magnum::Platform::GlfwApplication::KeyEvent& event);
        bool mousePressEvent(Magnum::Platform::GlfwApplication::MouseEvent& event);
        bool mouseReleaseEvent(Magnum::Platform::GlfwApplication::MouseEvent& event);
        bool mouseMoveEvent(Magnum::Platform::GlfwApplication::MouseMoveEvent& event);
        bool mouseScrollEvent(Magnum::Platform::GlfwApplication::MouseScrollEvent& event);
        bool textInputEvent(Magnum::Platform::GlfwApplication::TextInputEvent& event);

        Magnum::ImGuiIntegration::Context& getContext() { return _imguiContext; }
        void setEnable(bool enable) { _enable = enable; }
    private:
        bool _enable = true;
        std::unordered_set<GuiComponent*> _guiComponents;
        Magnum::ImGuiIntegration::Context _imguiContext{ Magnum::NoCreate };
        Magnum::Platform::GlfwApplication* _app{};
    };
}


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
        GuiSystem() { _exist = true; }
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

        void keyPressEvent(Magnum::Platform::GlfwApplication::KeyEvent& event);
        void keyReleaseEvent(Magnum::Platform::GlfwApplication::KeyEvent& event);
        void mousePressEvent(Magnum::Platform::GlfwApplication::MouseEvent& event);
        void mouseReleaseEvent(Magnum::Platform::GlfwApplication::MouseEvent& event);
        void mouseMoveEvent(Magnum::Platform::GlfwApplication::MouseMoveEvent& event);
        void mouseScrollEvent(Magnum::Platform::GlfwApplication::MouseScrollEvent& event);
        void textInputEvent(Magnum::Platform::GlfwApplication::TextInputEvent& event);

        Magnum::ImGuiIntegration::Context& getContext() { return _imguiContext; }
    private:
        std::unordered_set<GuiComponent*> _guiComponents;
        Magnum::ImGuiIntegration::Context _imguiContext{ Magnum::NoCreate };
        Magnum::Platform::GlfwApplication* _app{};
    };
}


#include "Game.h"
#include "Engine.h"
#include <Scene.h>

#include "yaml-cpp/yaml.h"

#include <Corrade/Containers/GrowableArray.h>
#include <Corrade/Containers/Optional.h>
#include <Corrade/Containers/Pointer.h>

#include <Magnum/Platform/GlfwApplication.h>

#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Version.h>
#include <Magnum/Math/Color.h>

#include "System/InputSystem.h"
#include "System/GuiSystem.h"
#include "System/DisplaySystem.h"
#include "System/PhysicsSystem.h"

#include "Component/CameraComponent.h"
#include "Component/ColoredDrawableComponent.h"
#include "Component/MeshGroupComponent.h"
#include "Component/RigidbodyComponent.h"
#include "Component/ShapeComponent.h"
#include "Component/TransformComponent.h"

using namespace Magnum;
using namespace Math::Literals;

namespace GDE
{
    Game::Game(
        const Arguments& arguments,
        string title,
        const Vector2i windowSize,
        WindowMode windowMode
    ):
    Platform::Application(arguments, NoCreate),
    _title(std::move(title)),
    _windowSize(windowSize)
    {
        // Application initialization
        {
            const Vector2 dpiScaling = this->dpiScaling({});
            Configuration config;
            GLConfiguration glConfig;

            Configuration::WindowFlag windowModeFlag;
            switch (windowMode) {
                case Windowed: default: windowModeFlag = Configuration::WindowFlag::Focused; break;
                case Maximized: windowModeFlag = Configuration::WindowFlag::Maximized; break;
                case Borderless: windowModeFlag = Configuration::WindowFlag::Borderless; break;
                case Fullscreen: windowModeFlag = Configuration::WindowFlag::Fullscreen; break;
            }

            config.setTitle(_title).setSize(_windowSize, dpiScaling).setWindowFlags(windowModeFlag | Configuration::WindowFlag::Resizable);
            glConfig.setSampleCount(dpiScaling.max() < 2.0f ? 8 : 2);

            if (!tryCreate(config, glConfig))
                create(config, glConfig.setSampleCount(0));
        }

        GL::Renderer::setClearColor(0x222222_rgbf);

        Debug{} << "Application running on"
            << GL::Context::current().version() << "using"
            << GL::Context::current().rendererString();

        if (_app == nullptr)
        {
            _app = this;
        }
    }

    Game::~Game()
    {
        for (auto& system : _systems)
        {
            if (auto* gui_system = dynamic_cast<GuiSystem*>(system))
            {
                gui_system->remove();
            }
            if (auto* display_system = dynamic_cast<DisplaySystem*>(system))
            {
                display_system->clean();
            } if (auto* physic_system = dynamic_cast<PhysicsSystem*>(system))
            {
                physic_system->clean();
            }
        }
    }

    void Game::registerBasicComponent() const
    {
        Component::Register<CameraComponent>();
        Component::Register<ColoredDrawableComponent>();
        Component::Register<MeshGroupComponent>();
        Component::Register<RigidbodyComponent>();
        Component::Register<ShapeComponent>();
        Component::Register<TransformComponent>();
    }

    void Game::keyPressEvent(KeyEvent& event)
    {
        if (event.isRepeated())
            return;
        if (GuiSystem::_exist)
            if (GuiSystem::getInstance().keyPressEvent(event))
                return;
        InputSystem::getInstance().setKeyState(static_cast<GDE::Key::Key>(event.key()), true);
    }

    void Game::keyReleaseEvent(KeyEvent& event)
    {
        if (event.isRepeated())
            return;
        if (GuiSystem::_exist)
            if (GuiSystem::getInstance().keyReleaseEvent(event))
                return;
        InputSystem::getInstance().setKeyState(static_cast<GDE::Key::Key>(event.key()), false);
    }

    void Game::mousePressEvent(MouseEvent& event)
    {
        if (GuiSystem::_exist)
            if (GuiSystem::getInstance().mousePressEvent(event))
                return;
        InputSystem::getInstance().setMouseButtonState(static_cast<GDE::Mouse::Button>(event.button()), true);
    }

    void Game::mouseReleaseEvent(MouseEvent& event)
    {
        if (GuiSystem::_exist)
            if (GuiSystem::getInstance().mouseReleaseEvent(event))
                return;
        InputSystem::getInstance().setMouseButtonState(static_cast<GDE::Mouse::Button>(event.button()), false);
    }

    void Game::mouseMoveEvent(MouseMoveEvent& event)
    {
        if (GuiSystem::_exist)
            if (GuiSystem::getInstance().mouseMoveEvent(event))
                return;

        InputSystem::getInstance().setMousePos(event.position());
        InputSystem::getInstance().setMouseVelocity(static_cast<Vector2>(event.relativePosition()));
    }

    void Game::mouseScrollEvent(MouseScrollEvent& event)
    {
        if (GuiSystem::_exist)
            if (GuiSystem::getInstance().mouseScrollEvent(event))
                return;
        InputSystem::getInstance().setMouseScrollValue(event.offset().y());
    }
    
    void Game::textInputEvent(TextInputEvent& event)
    {
        if (GuiSystem::_exist)
            if (GuiSystem::getInstance().textInputEvent(event))
                return;
    }

    void Game::viewportEvent(ViewportEvent& event)
    {
        GL::defaultFramebuffer.setViewport({ {}, event.framebufferSize() });
        if (GuiSystem::_exist)
            GuiSystem::getInstance().relayout(Vector2{ event.windowSize() } / dpiScaling(), event.windowSize(), framebufferSize());

    }

    void Game::setup()
    {
        setupSystem();
        addSystem<EntitySystem>();
        for (auto& system : _systems)
        {
            if (auto* gui_system = dynamic_cast<GuiSystem*>(system))
            {
                gui_system->setup(Vector2{ windowSize() } / dpiScaling(),
                    windowSize(), framebufferSize(), this);
            }
            if (auto* display_system = dynamic_cast<DisplaySystem*>(system))
            {
                display_system->setup();
            }

        }
        registerBasicComponent();
        registerComponent();
        setupLinkingContext();

        Scene::getEntityFromId(0);

        setupScene();

        _framerate_ms = static_cast<std::chrono::milliseconds>(static_cast<int>((1 / _fps) * 1000));
        _old_time = std::chrono::steady_clock::now();

    }

    void Game::drawEvent() {

        if (_init)
        {
            setup();
            _init = false;
        }

        auto current_time = std::chrono::steady_clock::now();
        auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - _old_time);

        if (dt < _framerate_ms)
        {
            std::this_thread::sleep_for(_framerate_ms - dt);
            current_time = std::chrono::steady_clock::now();
            dt = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - _old_time);
        }
        _old_time = current_time;
        _frame++;
        GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);
        for (auto& system : _systems)
        {
            system->iterate({ static_cast<float>(dt.count()), _frame});
        }

        swapBuffers();
        redraw();
    }
}
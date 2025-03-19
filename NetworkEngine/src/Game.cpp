#include "Game.h"
#include "Engine.h"
#include <Scene.h>

#include "yaml-cpp/yaml.h"
#include <Magnum/Platform/GlfwApplication.h>

#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Version.h>
#include <Magnum/Math/Color.h>

#include "System/InputSystem.h"
#include "System/GuiSystem.h"

#include <utility>

using namespace Magnum;
using namespace Math::Literals;

namespace GDE
{
    Game::Game(
        const Arguments& arguments,
        string title,
        const Vector2i windowSize
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

            config.setTitle(_title).setSize(_windowSize, dpiScaling).setWindowFlags(Configuration::WindowFlag::Maximized | Configuration::WindowFlag::Resizable);
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
        }
    }

    void Game::registerBasicComponent() const
    {
    }

    void Game::keyPressEvent(KeyEvent& event)
    {
        InputSystem::getInstance().setKeyState(static_cast<GDE::Key::Key>(event.key()), true);
        GuiSystem::getInstance().keyPressEvent(event);
    }

    void Game::keyReleaseEvent(KeyEvent& event)
    {
        InputSystem::getInstance().setKeyState(static_cast<GDE::Key::Key>(event.key()), false);
        GuiSystem::getInstance().keyPressEvent(event);
    }

    void Game::mousePressEvent(MouseEvent& event)
    {
        InputSystem::getInstance().setMouseButtonState(static_cast<GDE::Mouse::Button>(event.button()), true);
        GuiSystem::getInstance().mousePressEvent(event);
    }

    void Game::mouseReleaseEvent(MouseEvent& event)
    {
        InputSystem::getInstance().setMouseButtonState(static_cast<GDE::Mouse::Button>(event.button()), false);
        GuiSystem::getInstance().mouseReleaseEvent(event);
    }

    void Game::mouseMoveEvent(MouseMoveEvent& event)
    {
        InputSystem::getInstance().setMousePos(event.position());
        GuiSystem::getInstance().mouseMoveEvent(event);
    }

    void Game::mouseScrollEvent(MouseScrollEvent& event)
    {
        InputSystem::getInstance().setMouseScrollValue(event.offset().y());
        GuiSystem::getInstance().mouseScrollEvent(event);
    }
    
    void Game::textInputEvent(TextInputEvent& event)
    {
        GuiSystem::getInstance().textInputEvent(event);
    }

    void Game::viewportEvent(ViewportEvent& event)
    {
        GL::defaultFramebuffer.setViewport({ {}, event.framebufferSize() });
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
        }
        registerBasicComponent();
        registerComponent();

        setupScene();

        _framerate_ms = static_cast<std::chrono::milliseconds>(static_cast<int>((1 / _fps) * 1000));
        _old_time = std::chrono::high_resolution_clock::now();

    }

    void Game::drawEvent() {

        if (_init)
        {
            setup();
            _init = false;
        }

        auto current_time = std::chrono::high_resolution_clock::now();
        auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - _old_time);

        if (dt < _framerate_ms)
        {
            std::this_thread::sleep_for(_framerate_ms - dt);
            current_time = std::chrono::high_resolution_clock::now();
            dt = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - _old_time);
        }
        _old_time = current_time;

        GL::defaultFramebuffer.clear(GL::FramebufferClear::Color|GL::FramebufferClear::Depth);
        for (auto& system : _systems)
        {
            system->iterate({ static_cast<float>(dt.count()), _frame});
        }

        swapBuffers();
        redraw();
    }
}
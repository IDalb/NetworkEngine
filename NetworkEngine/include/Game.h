#pragma once

#include <Magnum/Platform/GlfwApplication.h>
#include <Magnum/GL/DefaultFramebuffer.h>

#include "Engine.h"
#include "Corrade/Utility/Arguments.h"
#include "source_dir.h"

using namespace std;
using namespace Magnum;
using namespace Math::Literals;

namespace GDE
{
    enum WindowMode {Windowed, Maximized, Borderless, Fullscreen};

    class Game : public Magnum::Platform::GlfwApplication
    {
    protected:
        // App properties
        const string _title;
        const Vector2i _windowSize;
        float _fps = 60.f;
        void setFps(const float fps) { _fps = fps; }
        void setup();
        virtual void setupScene() {}
        // Data
        std::vector<System*> _systems;
        template<typename T> void addSystem() { _systems.push_back(&T::getInstance()); }

        std::chrono::milliseconds _framerate_ms{};
        std::chrono::steady_clock::time_point _old_time{};
        unsigned int _frame = 0;
        // Methods
        void drawEvent() override;

        bool _init = true;
    public:
        inline static Game* _app = nullptr;

        explicit Game(
            const Arguments& arguments,
            string title = "Game",
            Vector2i windowSize = {500, 500},
            WindowMode windowMode = WindowMode::Windowed
        );
        virtual ~Game();

        virtual void setupSystem() = 0;
        virtual void registerComponent() const = 0;
        virtual void registerBasicComponent() const;

        virtual void keyPressEvent(KeyEvent& event) override;
        virtual void keyReleaseEvent(KeyEvent& event) override;
        virtual void mousePressEvent(MouseEvent& event) override;
        virtual void mouseReleaseEvent(MouseEvent& event) override;
        virtual void mouseMoveEvent(MouseMoveEvent& event) override;
        virtual void mouseScrollEvent(MouseScrollEvent& event) override;
        virtual void textInputEvent(TextInputEvent& event) override;

        virtual void viewportEvent(ViewportEvent& event) override;
    };
}

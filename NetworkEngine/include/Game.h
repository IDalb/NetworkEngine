#pragma once

#include <Magnum/Platform/GlfwApplication.h>

#include "Engine.h"
#include "Corrade/Utility/Arguments.h"

using namespace std;
using namespace Magnum;
using namespace Math::Literals;

namespace GDE
{
    class Game : public Magnum::Platform::Application
    {
    protected:
        // App properties
        const string _title;
        const Vector2i _windowSize;
        float _fps = 60.f;
        void setFps(const float fps) { _fps = fps; }

        // Data
        std::vector<System*> _systems;
        template<typename T> void addSystem() { _systems.push_back(&T::getInstance()); }

        // Methods
        void drawEvent() override;
    public:
        explicit Game(
            const Arguments& arguments,
            string title = "Game",
            Vector2i windowSize = {500, 500}
        );
        virtual ~Game() = default;

        virtual void setupSystem() = 0;
        virtual void registerComponent() const = 0;
    };
}

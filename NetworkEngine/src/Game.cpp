#include "Game.h"

#include "yaml-cpp/yaml.h"

#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Version.h>
#include <Magnum/Math/Color.h>

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
        {
            const Vector2 dpiScaling = this->dpiScaling({});
            Configuration config;
            GLConfiguration glConfig;

            config.setTitle(_title).setSize(_windowSize, dpiScaling);
            glConfig.setSampleCount(dpiScaling.max() < 2.0f ? 8 : 2);

            if (!tryCreate(config, glConfig))
                create(config, glConfig.setSampleCount(0));
        }

        GL::Renderer::setClearColor(0x222222_rgbf);

        Debug{} << "Application running on"
            << GL::Context::current().version() << "using"
            << GL::Context::current().rendererString();
    }

    void Game::drawEvent() {
        GL::defaultFramebuffer.clear(GL::FramebufferClear::Color|GL::FramebufferClear::Depth);
        swapBuffers();
    }
}
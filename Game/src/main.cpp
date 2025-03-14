#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/GlfwApplication.h>
#include "yaml-cpp/yaml.h"

#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Version.h>
#include <Magnum/Math/Color.h>

#include "Examples/BulletExample.cpp"

using namespace Magnum;
using namespace Math::Literals;

// ========== CLASS DEFINITIONS ========== //

class BaseApplication final : public Platform::Application {
public:
    explicit BaseApplication(const Arguments& arguments);
    virtual ~BaseApplication() = default;

private:
    void drawEvent() override;
};

// ========== CLASS IMPLEMENTATION ========== //

BaseApplication::BaseApplication(const Arguments& arguments): Platform::Application(arguments, NoCreate) {
{
    const Vector2 dpiScaling = this->dpiScaling({});
    Configuration config;
    config.setTitle("TP 3").setSize({700, 700}, dpiScaling);
    GLConfiguration glConfig;
    glConfig.setSampleCount(dpiScaling.max() < 2.0f ? 8 : 2);
    if (!tryCreate(config, glConfig))
        create(config, glConfig.setSampleCount(0));
}
    GL::Renderer::setClearColor(0x222222_rgbf);

    Debug{} << "Application running on"
        << GL::Context::current().version() << "using"
        << GL::Context::current().rendererString();
}

void BaseApplication::drawEvent() {
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color|GL::FramebufferClear::Depth);
    swapBuffers();
}

MAGNUM_APPLICATION_MAIN(BulletExample)

#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/GlfwApplication.h>

#include <Corrade/Containers/StringView.h>
#include <Magnum/GL/Context.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Version.h>
#include <Magnum/Math/Color.h>

#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Shaders/VertexColorGL.h>

using namespace Magnum;

class TriangleExample: public Platform::Application {
    public:
        explicit TriangleExample(const Arguments& arguments);

    private:
        void drawEvent() override;

        GL::Mesh _mesh;
        Shaders::VertexColorGL2D _shader;
};



TriangleExample::TriangleExample(const Arguments& arguments):
    Platform::Application{arguments, Configuration{}
    .setTitle("Magnum Triangle Example")
    .setSize({600, 600})
}
{
    using namespace Math::Literals;

    GL::Renderer::setClearColor(0x222222_rgbf);

    Debug{} << "Application running on"
        << GL::Context::current().version() << "using"
        << GL::Context::current().rendererString();
    
    struct TriangleVertex {
        Vector2 position;
        Color3 color;
    };

    const TriangleVertex vertices[] {
        {{-0.5f, -0.433f}, 0xff0000_rgbf},
        {{ 0.5f, -0.433f}, 0x00ff00_rgbf},
        {{ 0.0f,  0.433f}, 0x0000ff_rgbf}
    };

    _mesh.setCount(Containers::arraySize(vertices)).addVertexBuffer(
        GL::Buffer{vertices},
        0,
        Shaders::VertexColorGL2D::Position{},
        Shaders::VertexColorGL2D::Color3{});
}

void TriangleExample::drawEvent() {
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color|GL::FramebufferClear::Depth);

    _shader.draw(_mesh);
    
    swapBuffers();
}
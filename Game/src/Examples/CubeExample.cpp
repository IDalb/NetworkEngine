#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/GlfwApplication.h>
#include "yaml-cpp/yaml.h"

#include <Corrade/Containers/StringView.h>
#include <Magnum/GL/Context.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Version.h>
#include <Magnum/Math/Color.h>

#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Shaders/VertexColorGL.h>

#include <Magnum/Math/Angle.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Shaders/PhongGL.h>
#include <Magnum/Trade/MeshData.h>

using namespace Magnum;
using namespace Math::Literals;

class CubeExample: public Platform::Application {
    public:
        explicit CubeExample(const Arguments& arguments);

    private:
        void drawEvent() override;

        // Triangle
        GL::Mesh _mesh;
        Shaders::VertexColorGL2D _shader;

        // Cube
        void pointerReleaseEvent(PointerEvent& event) override;
        void pointerMoveEvent(PointerMoveEvent& event) override;

        GL::Mesh _cubeMesh;
        Shaders::PhongGL _cubeShader;

        Matrix4 _transformation, _projection;
        Color3 _color;
};

CubeExample::CubeExample(const Arguments& arguments):
    Platform::Application{arguments, Configuration{}
    .setTitle("Magnum 3D Cube Example")
    .setSize({600, 600})
}
{
    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

    GL::Renderer::setClearColor(0x222222_rgbf);

    Debug{} << "Application running on"
        << GL::Context::current().version() << "using"
        << GL::Context::current().rendererString();
    
    // Triangle
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
    
    // 3D Cube
    _cubeMesh = MeshTools::compile(Primitives::cubeSolid());

    _transformation = Matrix4::rotationX(30.0_degf)*Matrix4::rotationY(40.0_degf);
    _projection = Matrix4::perspectiveProjection(35.0_degf, Vector2{windowSize()}.aspectRatio(), 0.01f, 100.0f)
        * Matrix4::translation(Vector3::zAxis(-10.0f));
    _color = Color3::fromHsv({35.0_degf, 0.5f, 1.0f});
}

void CubeExample::drawEvent() {
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color|GL::FramebufferClear::Depth);

    _shader.draw(_mesh);

    _cubeShader.setLightPositions({{1.4f, 1.0f, 0.75f, 0.0f}})
        .setDiffuseColor(_color)
        .setAmbientColor(Color3::fromHsv({_color.hue(), 0.5f, 0.3f}))
        .setTransformationMatrix(_transformation)
        .setNormalMatrix(_transformation.normalMatrix())
        .setProjectionMatrix(_projection)
        .draw(_cubeMesh);
    
    swapBuffers();
}

void CubeExample::pointerReleaseEvent(PointerEvent &event)
{
    if (!event.isPrimary() || !(event.pointer() & Pointer::MouseLeft))
        return;
    
    _color = Color3::fromHsv({_color.hue() + 40.0_degf, .5f, 1.f});

    event.setAccepted();
    redraw();
}

void CubeExample::pointerMoveEvent(PointerMoveEvent& event) {
    if (!event.isPrimary() || !(event.pointers() & Pointer::MouseLeft))
        return;
    
    Vector2 delta = 3.0f * Vector2{event.relativePosition()} / Vector2{windowSize()};

    _transformation =
        Matrix4::rotationX(Rad{delta.y()}) *
        _transformation *
        Matrix4::rotationY(Rad{delta.x()});
    
    event.setAccepted();
    redraw();
}

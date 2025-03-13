#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/GlfwApplication.h>
#include "yaml-cpp/yaml.h"

#include <Corrade/Containers/StringView.h>
#include <Magnum/GL/Context.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Version.h>
#include <Magnum/Math/Color.h>

#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/Math/Time.h>

#include "BulletCollision/BroadphaseCollision/btDbvtBroadphase.h"
#include "BulletCollision/CollisionDispatch/btCollisionDispatcher.h"
#include "BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h"
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "Corrade/Containers/GrowableArray.h"
#include "Corrade/Containers/Pointer.h"
#include "Magnum/Timeline.h"
#include "Magnum/BulletIntegration/DebugDraw.h"
#include "Magnum/BulletIntegration/MotionState.h"
#include "Magnum/MeshTools/Compile.h"
#include "Magnum/MeshTools/Concatenate.h"
#include "Magnum/Primitives/Axis.h"
#include "Magnum/Primitives/Cube.h"
#include "Magnum/Primitives/UVSphere.h"
#include "Magnum/SceneGraph/Camera.h"
#include "Magnum/SceneGraph/Drawable.h"
#include "Magnum/SceneGraph/Scene.h"
#include "Magnum/Shaders/PhongGL.h"
#include "Magnum/Shaders/Shaders.h"

//#include <Corrade/Containers/GrowableArray.h>
//#include <Corrade/Containers/Optional.h>
//#include <Corrade/Containers/Pointer.h>

//#include <btBulletDynamicsCommon.h>
//#include <Magnum/Timeline.h>
//#include <Magnum/BulletIntegration/Integration.h>

#ifdef BT_USE_DOUBLE_PRECISION
#error Example does not support Bullet with double precision
#endif


using namespace Magnum;
using namespace Math::Literals;

typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D> Object3D;
typedef SceneGraph::Scene<SceneGraph::MatrixTransformation3D> Scene3D;

struct InstanceData {
    Matrix4 transformationMatrix;
    Matrix3x3 normalMatrix;
    Color3 color;
};


// ========== CLASS DEFINITIONS ========== //

class MainApplication: public Platform::Application {
public:
    explicit MainApplication(const Arguments& arguments);

private:
    void drawEvent() override;
    void keyPressEvent(KeyEvent &event) override;
    void pointerPressEvent(PointerEvent &event) override;

    GL::Mesh _box{NoCreate}, _sphere{NoCreate};
    GL::Buffer _boxInstanceBuffer{NoCreate}, _sphereInstanceBuffer{NoCreate};
    Shaders::PhongGL _shader{NoCreate};
    BulletIntegration::DebugDraw _debugDraw{NoCreate};
    Containers::Array<InstanceData> _boxInstanceData, _sphereInstanceData;

    btDbvtBroadphase _bBroadphase;
    btDefaultCollisionConfiguration _bCollisionConfig;
    btCollisionDispatcher _bDispatcher{&_bCollisionConfig};
    btSequentialImpulseConstraintSolver _bSolver;
    btDiscreteDynamicsWorld _bWorld{&_bDispatcher, &_bBroadphase, &_bSolver, &_bCollisionConfig};

    Scene3D _scene;
    SceneGraph::Camera3D* _camera;
    SceneGraph::DrawableGroup3D _drawables;
    Timeline _timeline;

    Object3D *_cameraRig, *_cameraObject;

    btBoxShape _bBoxShape{{.5f, .5f, .5f}};
    btSphereShape _bSphereShape{{.25f}};
    btBoxShape _bGroundShape{{8.f, .5f, 8.f}};

    bool _drawCubes, _drawDebug, _shootBox{true};
};

class ColoredDrawable: public SceneGraph::Drawable3D {
public:
    explicit ColoredDrawable(
        Object3D& object,
        Containers::Array<InstanceData>& instanceData,
        const Color3& color,
        const Matrix4& primitiveTransformation,
        SceneGraph::DrawableGroup3D& drawables):
    SceneGraph::Drawable3D(object, &drawables),
        _instanceData(instanceData),
        _color(color),
        _primitiveTransformation(primitiveTransformation)
    {}

private:
    void draw(const Matrix4& transformation, SceneGraph::Camera3D&) override {
        const Matrix4 t = transformation * _primitiveTransformation;
        arrayAppend(_instanceData, InPlaceInit, t, t.normalMatrix(), _color);
    }

    Containers::Array<InstanceData>& _instanceData;
    Color3 _color;
    Matrix4 _primitiveTransformation;
};

class Rigidbody: public Object3D {
public:
    Rigidbody(Object3D* parent, Float mass, btCollisionShape* bShape, btDynamicsWorld& bWorld)
        : Object3D(parent), _bWorld(bWorld)
    {
        // Calculate inertia
        btVector3 bInertia(0.f, 0.f, 0.f);
        if (!Math::TypeTraits<Float>::equals(mass, 0.f))
            bShape->calculateLocalInertia(mass, bInertia);

        // Setup Bullet rigidbody
        auto* motionState = new BulletIntegration::MotionState{*this};
        _bRigidbody.emplace(btRigidBody::btRigidBodyConstructionInfo {
            mass, &motionState->btMotionState(), bShape, bInertia});
        _bRigidbody->forceActivationState(DISABLE_DEACTIVATION);
        bWorld.addRigidBody(_bRigidbody.get());
    }

    ~Rigidbody() override {
        _bWorld.removeRigidBody(_bRigidbody.get());
    }

    btRigidBody rigidBody() { return *_bRigidbody; }

    void syncPose() {
        _bRigidbody->setWorldTransform(btTransform(transformationMatrix()));
    }

private:
    btDynamicsWorld& _bWorld;
    Containers::Pointer<btRigidBody> _bRigidbody;
};


// ========== METHOD IMPLEMENTATIONS ========== //

MainApplication::MainApplication(const Arguments& arguments): Platform::Application(arguments, NoCreate) {
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

    // Camera setup
    (_cameraRig = new Object3D(&_scene))->translate(Vector3::yAxis(3.0f)).rotateY(40.0_degf);
    (_cameraObject = new Object3D(_cameraRig))->translate(Vector3::zAxis(20.0f)).rotateX(-25.0_degf);
    (_camera = new SceneGraph::Camera3D(*_cameraObject))
        ->setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend)
        .setProjectionMatrix(Matrix4::perspectiveProjection(35.0_degf, 1.0f, 0.001f, 100.0f))
        .setViewport(GL::defaultFramebuffer.viewport().size());

    // Create a shader instance
    _shader = Shaders::PhongGL{Shaders::PhongGL::Configuration{}.setFlags(
            Shaders::PhongGL::Flag::VertexColor|Shaders::PhongGL::Flag::InstancedTransformation)
    };
    _shader.setAmbientColor(0x222222_rgbf)
    .setSpecularColor(0x004400_rgbf)
    .setLightPositions({{10.0f, 15.0f, 5.0f, 0.0f}});

    // Box and sphere mesh initialization
    _box = MeshTools::compile(Primitives::cubeSolid());
    _sphere = MeshTools::compile(Primitives::uvSphereSolid(16, 32));
    _boxInstanceBuffer = GL::Buffer{};
    _sphereInstanceBuffer = GL::Buffer{};
    _box.addVertexBufferInstanced(_boxInstanceBuffer, 1, 0,
        Shaders::PhongGL::TransformationMatrix{},
        Shaders::PhongGL::NormalMatrix{},
        Shaders::PhongGL::Color3{});
    _sphere.addVertexBufferInstanced(_sphereInstanceBuffer, 1, 0,
        Shaders::PhongGL::TransformationMatrix{},
        Shaders::PhongGL::NormalMatrix{},
        Shaders::PhongGL::Color3{});


    // Setup the renderer
    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
    GL::Renderer::enable(GL::Renderer::Feature::PolygonOffsetFill);
    GL::Renderer::setPolygonOffset(2.0f, 0.5f);

    // Bullet (physics) setup
    _debugDraw = BulletIntegration::DebugDraw{};
    _debugDraw.setMode(BulletIntegration::DebugDraw::Mode::DrawWireframe);
    _bWorld.setGravity({0.f, -10.f, 0.f});
    _bWorld.setDebugDrawer(&_debugDraw);


    // ===== Create objects ===== //
    // Ground
    auto* ground = new Rigidbody(&_scene, 0.f, &_bGroundShape, _bWorld);
    new ColoredDrawable{
        *ground,
        _boxInstanceData,
        0xffffff_rgbf,
        Matrix4::scaling({8.f, .5f, 8.f}),
        _drawables
    };

    // Cubes
    Deg hue = 42.0_degf;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 5; k++) {
                auto* obj = new Rigidbody(&_scene, 1.0f, &_bBoxShape, _bWorld);
                obj->translate({i - 2.f, j + 4.f, k - 2.f});
                obj->syncPose();
                new ColoredDrawable{
                    *obj,
                    _boxInstanceData,
                    Color3::fromHsv({hue += 137.5_degf, 0.75f, 0.9f}),
                    Matrix4::scaling(Vector3{.5f}),
                    _drawables
                };
            }
        }
    }

    // Set framerate
    setSwapInterval(1);
    setMinimalLoopPeriod(16.0_msec);
    _timeline.start();
}

void MainApplication::drawEvent() {
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color|GL::FramebufferClear::Depth);

    // Removes objects that are too far from the origin
    for (Object3D* obj = _scene.children().first(); obj;) {
        Object3D* next = obj->nextSibling();
        if (obj->transformation().translation().dot() > 100*100)
            delete obj;

        obj = next;
    }

    // Physics engine
    _bWorld.stepSimulation(_timeline.previousFrameDuration(), 5);

    // Draw objects
    if (_drawCubes) {
        arrayResize(_boxInstanceData, 0);
        arrayResize(_sphereInstanceData, 0);
        _camera->draw(_drawables);

        _shader.setProjectionMatrix(_camera->projectionMatrix());

        // Upload data to the GPU and draw all cubes at once
        _boxInstanceBuffer.setData(_boxInstanceData, GL::BufferUsage::DynamicDraw);
        _box.setInstanceCount(_boxInstanceData.size());
        _shader.draw(_box);

        _sphereInstanceBuffer.setData(_sphereInstanceData, GL::BufferUsage::DynamicDraw);
        _sphere.setInstanceCount(_sphereInstanceData.size());
        _shader.draw(_sphere);
    }

    // Debug Draw
    if (_drawDebug) {
        if (_drawCubes)
            GL::Renderer::setDepthFunction(GL::Renderer::DepthFunction::LessOrEqual);

        _debugDraw.setTransformationProjectionMatrix(_camera->projectionMatrix() * _camera->cameraMatrix());
        _bWorld.debugDrawWorld();

        if (_drawCubes)
            GL::Renderer::setDepthFunction(GL::Renderer::DepthFunction::Less);
    }

    swapBuffers();
    _timeline.nextFrame();
    redraw();
}

// ========== Event methods ========== //

void MainApplication::keyPressEvent(KeyEvent &event) {
    // Movement
    if (event.key() == Key::Down)
        _cameraObject->rotateX(5.0_degf);
    else if (event.key() == Key::Up)
        _cameraObject->rotateX(-5.0_degf);
    else if (event.key() == Key::Left)
        _cameraObject->rotateY(-5.0_degf);
    else if (event.key() == Key::Right)
        _cameraObject->rotateY(5.0_degf);

    // Toggle draw modes
    else if (event.key() == Key::D) {
        if (_drawCubes && _drawDebug)
            _drawDebug = false;
        else if(_drawCubes && !_drawDebug) {
            _drawCubes = false;
            _drawDebug = true;
        } else if(!_drawCubes && _drawDebug) {
            _drawCubes = true;
            _drawDebug = true;
        }
    }

    // Switch selected projectile
    else if (event.key() == Key::S)
        _shootBox ^= true;

    else return;

    event.setAccepted();
}

void MainApplication::pointerPressEvent(PointerEvent &event) {
    // Shoot on click
    if (!event.isPrimary())
        return;

    // Set up the object transform
    const Vector2 position = event.position() * Vector2{framebufferSize()}/Vector2{windowSize()};
    const Vector2 clickPoint = Vector2::yScale(-1.0f) * (position/Vector2{framebufferSize()} - Vector2{0.5f}) * _camera->projectionSize();
    const Vector3 direction = (_cameraObject->absoluteTransformation().rotationScaling()*Vector3{clickPoint, -1.0f}).normalized();

    auto* object = new Rigidbody {
        &_scene,
        _shootBox ? 1.0f : 5.0f,
        _shootBox ? static_cast<btCollisionShape*>(&_bBoxShape) : &_bSphereShape,
        _bWorld
    };
    object->translate(_cameraObject->absoluteTransformation().translation());
    object->syncPose();

    // Create shape
    new ColoredDrawable{*object,
        _shootBox ? _boxInstanceData : _sphereInstanceData,
        _shootBox ? 0x880088_rgbf : 0x002222_rgbf,
        Matrix4::scaling(Vector3{_shootBox ? 0.5f : 0.25f}),
        _drawables
    };

    // Give object an initial velocity
    object->rigidBody().setLinearVelocity(btVector3{direction * 25.f});

    event.setAccepted();
}

MAGNUM_APPLICATION_MAIN(MainApplication)

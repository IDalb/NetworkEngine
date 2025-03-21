#include "Game.h"
#include <System/DisplaySystem.h>
#include <System/GuiSystem.h>
#include <System/InputSystem.h>
#include <System/LogicSystem.h>
#include <System/PhysicsSystem.h>

#include "Component/BallShootComponent.h"
namespace Game
{
    class App final : public GDE::Game
    {
    public:
        explicit App(const Arguments& arguments);

        void setupSystem() override;
        void registerComponent() const override;
        void setupScene() override;
    };

    App::App(const Arguments& arguments) : Game(
        arguments,
        "TP3 Game",
        { 700, 700 },
        GDE::Windowed
    )
    {
    }

    void App::setupSystem()
    {
        addSystem<GDE::LogicSystem>();
        addSystem<GDE::PhysicsSystem>();
        addSystem<GDE::DisplaySystem>();
        addSystem<GDE::InputSystem>();

        GDE::DisplaySystem::getInstance().initImpl([]{ return std::make_unique<GDE::DisplaySystemImpl>(); });
    }

    void App::registerComponent() const
    {
        GDE::Component::Register<BallShootComponent>();
    }

    void App::setupScene()
    {
        GDE::Scene::load(GDE::Descr::load(std::string(SOURCE_DIR) + "/Game/data/scene/scene.yaml"));
    }
}

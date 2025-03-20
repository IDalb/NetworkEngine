#include "Game.h"
#include <System/DisplaySystem.h>
#include <System/GuiSystem.h>
#include <System/InputSystem.h>
#include <System/LogicSystem.h>
#include <System/PhysicsSystem.h>

#include "Component/BallShootComponent.h"
namespace Game
{
    class TPGame final : public GDE::Game
    {
    public:
        explicit TPGame(const Arguments& arguments);

        void setupSystem() override;
        void registerComponent() const override;
        void setupScene() override;
    };

    TPGame::TPGame(const Arguments& arguments) : Game(
        arguments,
        "TP3 Game",
        { 700, 700 },
        GDE::Windowed
    )
    {
    }

    void TPGame::setupSystem()
    {
        addSystem<GDE::LogicSystem>();
        addSystem<GDE::PhysicsSystem>();
        addSystem<GDE::DisplaySystem>();
        addSystem<GDE::InputSystem>();
    }

    void TPGame::registerComponent() const
    {
        GDE::Component::Register<BallShootComponent>();
    }

    void TPGame::setupScene()
    {
        GDE::Scene::load(GDE::Descr::load(std::string(SOURCE_DIR) + "/Game/data/scene/scene.yaml"));
    }
}

MAGNUM_APPLICATION_MAIN(Game::TPGame);
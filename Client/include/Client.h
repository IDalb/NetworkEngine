#include "Game.h"
#include <System/DisplaySystem.h>
#include <System/GuiSystem.h>
#include <System/InputSystem.h>
#include <System/LogicSystem.h>
#include <System/PhysicsSystem.h>
#include <System/ClientNetworkSystem.h>
namespace Client
{
    class Client final : public GDE::Game
    {
    public:
        explicit Client(const Arguments& arguments);

        void setupSystem() override;
        void registerComponent() const override;
        void setupScene() override;
    };

    Client::Client(const Arguments& arguments) : Game(
        arguments,
        "Client",
        { 700, 700 },
        GDE::Windowed
    )
    {
    }

    void Client::setupSystem()
    {
        addSystem<GDE::LogicSystem>();
        addSystem<GDE::PhysicsSystem>();
        addSystem<GDE::DisplaySystem>();
        addSystem<GDE::InputSystem>();
        addSystem<GDE::ClientNetworkSystem>();


        GDE::PhysicsSystem::getInstance().setEnable(false);
        GDE::DisplaySystem::getInstance().initImpl([]{ return std::make_unique<GDE::DisplaySystemImpl>(); });

        this_thread::sleep_for(100ms);
        GDE::ClientNetworkSystem::getInstance().connect("127.0.0.1", 1234, GDE::NetworkAddressType::NETWORK_ADDRESS_TYPE_ANY);
    }

    void Client::registerComponent() const
    {
    }

    void Client::setupScene()
    {
        GDE::Scene::load(GDE::Descr::load(std::string(SOURCE_DIR) + "/Client/data/scene/scene.yaml"));
    }
}

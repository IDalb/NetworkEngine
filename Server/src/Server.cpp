#include "Server.h"

namespace Server
{
    Server::Server(const Arguments& arguments) : Game(
        arguments,
        "Server",
        { 700, 700 },
        GDE::Windowed
    )
    {
    }

    void Server::setupSystem()
    {
        addSystem<GDE::LogicSystem>();
        addSystem<GDE::PhysicsSystem>();
        addSystem<GDE::DisplaySystem>();
        addSystem<GDE::ServerInputSystem>();
        addSystem<GDE::InterpolationSystem>();
        addSystem<GDE::ServerNetworkSystem>();

        GDE::DisplaySystem::getInstance().initImpl([] { return std::make_unique<GDE::DisplaySystemImpl>(); });
        GDE::DisplaySystem::getInstance().setEnable(false);

        GDE::InterpolationSystem::getInstance().setEnable(false);

        GDE::ServerNetworkSystem::getInstance().createServer(4, 1234, GDE::NetworkAddressType::NETWORK_ADDRESS_TYPE_IPV6);
    }
    void Server::setupLinkingContext()
    {
        auto& linkingContext = GDE::LinkingContext<GDE::NetworkTemplateSize>::getInstance();

        linkingContext.addTemplate("Cube.yaml");
        linkingContext.addTemplate("Ground.yaml");
        linkingContext.addTemplate("Player.yaml");
        linkingContext.addTemplate("Ball.yaml");

    }
    void Server::registerComponent() const
    {
        GDE::Component::Register<BallShootComponent>();
        GDE::Component::Register<Player>();
        GDE::Component::Register<Object>();
        GDE::Component::Register<GameManager>();
    }

    void Server::setupScene()
    {
        GDE::Scene::load(GDE::Descr::load(std::string(SOURCE_DIR) + "/Server/data/scene/scene.yaml"));
    }
}
#include "Game.h"
#include <System/DisplaySystem.h>
#include <System/GuiSystem.h>
#include <System/InputSystem.h>
#include <System/LogicSystem.h>
#include <System/PhysicsSystem.h>
#include <System/ServerNetworkSystem.h>
#include <TypeDef.h>
#include <Utils/LinkingContext.h>
#include "Component/BallShootComponent.h"

namespace Server
{
    class Server final : public GDE::Game
    {
    public:
        explicit Server(const Arguments& arguments);

        void setupSystem() override;
        void registerComponent() const override;
        void setupScene() override;
        void setupLinkingContext() override;
        virtual std::string getTemplatePath() const override { return std::string(SOURCE_DIR) + "/Server/data/template/"; }
    };

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
        addSystem<GDE::InputSystem>();
        addSystem<GDE::ServerNetworkSystem>();

        GDE::DisplaySystem::getInstance().initImpl([]{ return std::make_unique<GDE::DisplaySystemImpl>(); });

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
    }

    void Server::setupScene()
    {
        GDE::Scene::load(GDE::Descr::load(std::string(SOURCE_DIR) + "/Server/data/scene/scene.yaml"));
    }
    
}

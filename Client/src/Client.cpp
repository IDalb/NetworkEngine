#include "Client.h"
#include <Component/ColoredDrawableComponent.h>
#include <GlobalConstants.h>
namespace Client
{

    Client::Client(const Arguments& arguments) : Game(
        arguments,
        "Client",
        { 700, 700 },
        GDE::Windowed
    )
    {
        GDE::Description apiConfig = GDE::Descr::load(std::string(SOURCE_DIR) + "/config.yaml");
        std::string ip = apiConfig["api_ip"].as<std::string>();
        std::string port = apiConfig["api_port"].as<std::string>();

        constants::WEB_API_URL = "http://" + ip + ":" + port + "/";
    }

    void Client::setupSystem()
    {
        addSystem<GDE::LogicSystem>();
        addSystem<GDE::GuiSystem>();
        addSystem<GDE::PhysicsSystem>();
        addSystem<GDE::DisplaySystem>();
        addSystem<GDE::InterpolationSystem>();
        addSystem<GDE::ClientNetworkSystem>();
        addSystem<GDE::InputSystem>();


        GDE::PhysicsSystem::getInstance().setEnable(false);
        GDE::DisplaySystem::getInstance().initImpl([] { return std::make_unique<GDE::DisplaySystemImpl>(); });

        //GDE::InterpolationSystem::getInstance().setEnable(false);
    }

    void Client::setupLinkingContext()
    {
        auto& linkingContext = GDE::LinkingContext<GDE::NetworkTemplateSize>::getInstance();

        linkingContext.addTemplate("Cube.yaml");
        linkingContext.addTemplate("Ground.yaml");
        linkingContext.addTemplate("Player.yaml");
        linkingContext.addTemplate("Ball.yaml");

    }


    void Client::registerComponent() const
    {
        GDE::Component::Register<LoginMenu>();
        GDE::Component::Register<PlayMenu>();
    }

    void Client::setupScene()
    {
        GDE::Scene::load(GDE::Descr::load(std::string(SOURCE_DIR) + "/Client/data/scene/LoginMenu.yaml"));
    }

    void Client::startGame()
    {
        GDE::Scene::clear();
        GDE::Scene::load(GDE::Descr::load(std::string(SOURCE_DIR) + "/Client/data/scene/Scene.yaml"));

        auto players = GDE::Scene::findEntitiesWithTag("player");
        for (auto& player : *players)
        {
            if (player->getComponent<GDE::ColoredDrawableComponent>()->getColor() == PLAYERS_COLOR[GDE::ClientNetworkSystem::getInstance()._gameId])
            {
                GDE::Scene::createEntity(GDE::Descr::load(std::string(SOURCE_DIR) + "/Client/data/template/Camera.yaml")[0], player->shared_from_this(), GDE::Scene::CLIENT_SIDE_ONLY);
                break;
            }
        }
    }
    void Client::endGame()
    {
        GDE::Scene::clear();
        GDE::Scene::load(GDE::Descr::load(std::string(SOURCE_DIR) + "/Client/data/scene/PlayMenu.yaml"));
    }
}
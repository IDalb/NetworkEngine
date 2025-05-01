#pragma once
#include "Game.h"
#include <System/DisplaySystem.h>
#include <System/GuiSystem.h>
#include <System/ServerInputSystem.h>
#include <System/LogicSystem.h>
#include <System/PhysicsSystem.h>
#include <System/ServerNetworkSystem.h>
#include <System/InterpolationSystem.h>
#include <TypeDef.h>
#include <Utils/LinkingContext.h>
#include "Component/BallShootComponent.h"
#include "Component/Object.h"
#include "Component/GameManager.h"
#include "Component/Player.h"

namespace Server
{
    constexpr std::array<Magnum::Vector3, 4> PLAYERS_COLOR = { Magnum::Vector3(1,0,0), Magnum::Vector3(0,1,0) , Magnum::Vector3(0,0,1) , Magnum::Vector3(1,1,0) };

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
}

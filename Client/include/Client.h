#pragma once
#include "Game.h"
#include <System/DisplaySystem.h>
#include <System/GuiSystem.h>
#include <System/InputSystem.h>
#include <System/LogicSystem.h>
#include <System/PhysicsSystem.h>
#include <System/ClientNetworkSystem.h>
#include <System/GuiSystem.h>
#include <System/InterpolationSystem.h>

#include <Utils/LinkingContext.h>
#include "Component/LoginMenu.h"
#include "Component/PlayMenu.h"
namespace Client
{
    constexpr std::array<Magnum::Vector3, 4> PLAYERS_COLOR = { Magnum::Vector3(1,0,0), Magnum::Vector3(0,1,0) , Magnum::Vector3(0,0,1) , Magnum::Vector3(1,1,0) };

    class Client final : public GDE::Game
    {
    public:
        explicit Client(const Arguments& arguments);

        void setupSystem() override;
        void registerComponent() const override;
        void setupScene() override;
        void setupLinkingContext() override;
        virtual std::string getTemplatePath() const override { return std::string(SOURCE_DIR) + "/Client/data/template/"; }
        void startGame() override;
        void endGame() override;
    };
}

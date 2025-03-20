#include "Game.h"
#include "System/DisplaySystem.h"
#include "System/GuiSystem.h"
#include "System/InputSystem.h"
#include "System/PhysicsSystem.h"

class TPGame final : public GDE::Game
{
public:
    explicit TPGame(const Arguments& arguments);

    void setupSystem() override;
    void registerComponent() const override;
};

TPGame::TPGame(const Arguments& arguments): Game(
    arguments,
    "TP3 Game",
    {700, 700},
    GDE::Windowed
) {}

void TPGame::setupSystem() {
    addSystem<GDE::EntitySystem>();
    addSystem<GDE::PhysicsSystem>();
    addSystem<GDE::DisplaySystem>();
    addSystem<GDE::GuiSystem>();
    addSystem<GDE::InputSystem>();
}

void TPGame::registerComponent() const {

}

MAGNUM_APPLICATION_MAIN(TPGame);
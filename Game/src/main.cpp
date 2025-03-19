#include "Game.h"

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
    {700, 700}
) {}

void TPGame::setupSystem() {
    // TODO: Implementation
}

void TPGame::registerComponent() const {
    // TODO: Implementation
}

MAGNUM_APPLICATION_MAIN(TPGame);
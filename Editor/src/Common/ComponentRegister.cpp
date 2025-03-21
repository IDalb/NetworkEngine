#pragma once
#include "Common/ComponentRegister.h"

void registerCommonComponent()
{
	GDE::Component::Register<Game::BallShootComponent>();
}


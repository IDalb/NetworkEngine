#include "Component/Object.h"
#include <Entity.h>
#include <Component/TransformComponent.h>
#include <Component/ColoredDrawableComponent.h>
#include <Component/GameManager.h>
#include "Server.h"
void Server::Object::updateLogic(const GDE::Timing& timing)
{
	if (owner().getComponent<GDE::TransformComponent>()->getTransform().transformation().translation().z() < -2)
	{
		const Magnum::Vector3& color = owner().getComponent<GDE::ColoredDrawableComponent>()->getColor();
		for (int i = 0; i < 4; i++)
		{
			if (PLAYERS_COLOR[i] == color)
			{
				GDE::Scene::findEntityWithTag("gameManager")->getComponent<GameManager>()->addPoint(i);
				break;
			}
		}
		owner().getParent()->removeChild(owner());
	}
}

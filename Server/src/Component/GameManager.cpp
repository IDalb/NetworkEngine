#include "Component/GameManager.h"
#include <System/ServerNetworkSystem.h>
void Server::GameManager::addPoint(uint32_t player)
{
	_points[player]++;
	if (_points[player] == 4)
	{
		saveStats(player);
		GDE::ServerNetworkSystem::getInstance().disconnectEverybody();
	}
}

void Server::GameManager::saveStats(uint32_t winner)
{
	// API CALL
	for (int i = 0; i < 4; i++)
	{
		uint32_t playerNetId = GDE::ServerNetworkSystem::getInstance()._gameIdToNetId[i];
		//send score
		// score : _points[i]
		// increment game player
		if (winner == i)
		{
			// increment game won
		}
	}
}

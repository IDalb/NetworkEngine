#include "Component/GameManager.h"

#include <iostream>
#include <System/ServerNetworkSystem.h>

#include "Server.h"
#include <cpr/cpr.h>
#include <Utils/json.hpp>
#include <GlobalConstants.h>

void Server::GameManager::addPoint(uint32_t player)
{
	_points[player]++;
	if (_points[player] == 4)
	{
		saveStats(player);
		GDE::ServerNetworkSystem::getInstance().disconnectEverybody();
		resetServer();
	}
}

void Server::GameManager::saveStats(uint32_t winner)
{
	// API CALL : save stats
	for (int i = 0; i < GDE::ServerNetworkSystem::getInstance()._gameIdToNetId.size(); i++)
	{
		uint32_t playerNetId = GDE::ServerNetworkSystem::getInstance()._gameIdToNetId[i];

		std::ostringstream request, update_request;
		std::ostringstream request_body;

		// Get data
		request << std::string(constants::WEB_API_URL)
			<< "stats/user/"
			<< playerNetId;

		// Total ejected cubes
		cpr::Response rCubes = Get(
			cpr::Url{request.str()},
			cpr::Parameters{{"statName", "cubesExpelled"}}
		);
		float cubes = static_cast<float>(nlohmann::json::parse(rCubes.text));

		// Played games
		cpr::Response rPlayed = Get(
			cpr::Url{request.str()},
			cpr::Parameters{{"statName", "gamesPlayed"}}
		);
		float played = static_cast<float>(nlohmann::json::parse(rPlayed.text));

		// Won games
		cpr::Response rWon = Get(
			cpr::Url{request.str()},
			cpr::Parameters{{"statName", "gamesWon"}}
		);
		float won = static_cast<float>(nlohmann::json::parse(rWon.text));

		// Update values
		update_request << std::string(constants::WEB_API_URL) << "stats/" << playerNetId;

		// Cubes expelled
		request_body.clear();
		request_body << "{"
		   << R"("name": "cubesExpelled")" << ", "
		   << R"("value": )" << cubes + _points[i]
		   << "}";
		cpr::Response rCubesPost = Put(
			cpr::Url{ update_request.str()},
			cpr::Header{{"Content-Type", "application/json"}},
			cpr::Body{request_body.str()}
		);
		if (rCubesPost.status_code != 204)
			cout << "Error: " << rCubesPost.status_code << " when updating stat 'cubesExpelled'"<< endl;

		// Games played
		request_body.str("");
		request_body.clear();
		request_body << "{"
		   << R"("name": "gamesPlayed")" << ", "
		   << R"("value": )" << played + 1
		   << "}";
		cpr::Response rPlayedPost = Put(
			cpr::Url{ update_request.str()},
			cpr::Header{{"Content-Type", "application/json"}},
			cpr::Body{request_body.str()}
		);
		if (rPlayedPost.status_code != 204)
			cout << "Error: " << rCubesPost.status_code << " when updating stat 'cubesExpelled'"<< endl;

		// Gams won
		if (winner == i)
		{
			request_body.str("");
			request_body.clear();
			request_body << "{"
			   << R"("name": "gamesWon")" << ", "
			   << R"("value": )" << won + 1
			   << "}";
			cpr::Response rWonPost = Put(
				cpr::Url{ update_request.str()},
				cpr::Header{{"Content-Type", "application/json"}},
				cpr::Body{request_body.str()}
			);
			if (rWonPost.status_code != 204)
				cout << "Error: " << rCubesPost.status_code << " when updating stat 'cubesExpelled'"<< endl;
		}
	}
}

void Server::GameManager::resetServer() {

}


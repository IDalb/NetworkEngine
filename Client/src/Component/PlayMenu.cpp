#include "Component/PlayMenu.h"
#include <imgui.h>
#include <iostream>
#include <Scene.h>
#include <System/ClientNetworkSystem.h>
#include <cpr/cpr.h>
#include <Utils/json.hpp>
#include "Client.h"

void Client::PlayMenu::setup(const GDE::ComponentDescription& init_value)
{
    // Achievements setup
    _achievements.clear();

    // API CALL : achievements
    ostringstream request;
    request << std::string(WEB_API_URL)
        << "achievements/user/"
        << GDE::ClientNetworkSystem::getInstance()._netId;

    if (cpr::Response response = Get(cpr::Url{request.str()}); response.status_code == 200) {
        nlohmann::json r_data = nlohmann::json::parse(response.text);
        auto achievements_info = YAML::LoadFile(std::string(SOURCE_DIR) + "/Client/data/achievements.yaml");

        for (int i = 0; i < r_data.size(); i++) {
            if (auto info = achievements_info[static_cast<string>(r_data[i])]) {
                _achievements.push_back(info["name"].as<string>() + ": " + info["description"].as<string>());
            }
            else {
                _achievements.push_back(r_data[i]);
            }
        }
    }
    else {
        _achievements.push_back("Achievements fetching error");
    }
}

void Client::PlayMenu::update(const GDE::Timing& dt)
{
    ImGui::Begin("Main Menu");

    // Calculate width for panels (third of window)
    float panelWidth = ImGui::GetContentRegionAvail().x * 0.4f;

    // Left Panel: Play Button
    ImGui::BeginChild("LeftPanel", ImVec2(panelWidth, 0), true);
    bool endDisable = false;
    if (_waiting)
    {
        ImGui::BeginDisabled();
        endDisable = true;
    }

    if (ImGui::Button("Play", ImVec2(-FLT_MIN, 0))) 
    {
        // API CALL : get server
        // get ip and port to connect to is
        cpr::Response rGetServer = Get(cpr::Url{std::string(WEB_API_URL) + "matches/find"});

        if (rGetServer.status_code != 200) {
            _serverError = true;
        }
        else {
            nlohmann::json r_data = nlohmann::json::parse(rGetServer.text);
            const std::string ip = r_data["ip"];
            const uint16_t port = r_data["port"];

            cout << "IP address: " << ip << " Port: " << port << endl;

            // API CALL : make player join the server
            ostringstream request;
            request << std::string(WEB_API_URL)
                << "matches/connect/"
                << r_data["id"];

            ostringstream playerId; playerId << GDE::ClientNetworkSystem::getInstance()._netId;
            cpr::Response rJoinServer = Post(
                cpr::Url{request.str()},
                cpr::Parameters{{"playerId", playerId.str()}}
            );
            if (rJoinServer.status_code != 200) {
                cout << "Error " << rJoinServer.status_code << " when trying to join the server" << endl;
            }

            GDE::ClientNetworkSystem::getInstance().connect(ip, port, GDE::NetworkAddressType::NETWORK_ADDRESS_TYPE_ANY);
            _waiting = true;
        }
    }
    if (_serverError)
    {
        ImGui::Text("Unable to find a server");
    }
    if (endDisable)
    {
        ImGui::EndDisabled();
        ImGui::Text("Waiting for players...");
    }

    ImGui::EndChild();

    ImGui::SameLine(); // Put the next panel to the right

    // Right Panel: Achievements list
    ImGui::BeginChild("RightPanel", ImVec2(0, 0), true);
    ImGui::Text("Achievements:");
    ImGui::Separator();
    for (const auto& achievement : _achievements) {
        ImGui::BulletText("%s", achievement.c_str());
    }
    ImGui::EndChild();

    ImGui::End();
}

#include "Component/PlayMenu.h"
#include <imgui.h>
#include <Scene.h>
#include "source_dir.h"
#include <System/ClientNetworkSystem.h>

void Client::PlayMenu::setup(const GDE::ComponentDescription& init_value)
{
    // API CALL
    //remplire _achievements avec les achievements
}

void Client::PlayMenu::update(const GDE::Timing& dt)
{// Dummy achievements data

    ImGui::Begin("Main Menu");

    // Calculate width for panels (half of window)
    float panelWidth = ImGui::GetContentRegionAvail().x * 0.5f;

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
        // API CALL
        // get server ip and port to connect to is
        const std::string ip = "127.0.0.1";
        const uint16_t port = 1234;

        GDE::ClientNetworkSystem::getInstance().connect(ip, port, GDE::NetworkAddressType::NETWORK_ADDRESS_TYPE_ANY);
        _waiting = true;
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

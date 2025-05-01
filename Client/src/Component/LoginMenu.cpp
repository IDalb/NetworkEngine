#include "Component/LoginMenu.h"
#include <imgui.h>
#include <Scene.h>
#include "source_dir.h"
#include <System/ClientNetworkSystem.h>
void Client::LoginMenu::setup(const GDE::ComponentDescription& init_value)
{
}

void Client::LoginMenu::update(const GDE::Timing& dt)
{
    static char pseudo[64] = "";
    static char password[64] = "";

    ImGui::Begin("Login");

    ImGui::InputText("Pseudo", pseudo, IM_ARRAYSIZE(pseudo));

    // Password field (masking input)
    ImGui::InputText("Password", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);

    if (ImGui::Button("Login")) {
        // API CALL
        // mot de passe et pseudo entré, vérification des valeurs
        srand(time(NULL));
        uint32_t playerId = rand();
        if (playerId != 0)
        {
            GDE::ClientNetworkSystem::getInstance()._netId = playerId;
            GDE::Scene::clear();
            GDE::Scene::load(GDE::Descr::load(std::string(SOURCE_DIR) + "/Client/data/scene/PlayMenu.yaml"));
        }
        else
        {
            _loginError = true;
        }
    }
    if (_loginError)
    {
        ImGui::Text("login or password incorrect");
    }
    ImGui::End();
}

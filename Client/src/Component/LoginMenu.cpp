#include "Component/LoginMenu.h"
#include <imgui.h>
#include <Scene.h>
#include "source_dir.h"
#include <System/ClientNetworkSystem.h>
#include <cpr/cpr.h>

#include "Client.h"
#include "Utils/json.hpp"

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
        // API CALL : login
        std::ostringstream request_body;
        request_body << "{"
           << R"("username": )" << std::quoted(pseudo) << ", "
           << R"("password": )" << std::quoted(password)
           << "}";

        cpr::Response r = cpr::Post(
            cpr::Url{std::string(WEB_API_URL) + "login"},
            cpr::Header{{"Content-Type", "application/json"}},
            cpr::Body{request_body.str()}
        );

        if (r.status_code == 200) {
            nlohmann::json r_data = nlohmann::json::parse(r.text);
            const uint32_t playerId = r_data["id"];
            const std::string token = r_data["token"];

            if (playerId != 0)
            {
                GDE::ClientNetworkSystem::getInstance()._netId = playerId;
                GDE::ClientNetworkSystem::getInstance()._netToken = token;
                GDE::Scene::clear();
                GDE::Scene::load(GDE::Descr::load(std::string(SOURCE_DIR) + "/Client/data/scene/PlayMenu.yaml"));
            }
            else
            {
                _loginError = true;
            }
        }
        else {
            _loginError = true;
        }
    }
    if (_loginError)
    {
        ImGui::Text("login or password incorrect");
    }
    ImGui::End();
}

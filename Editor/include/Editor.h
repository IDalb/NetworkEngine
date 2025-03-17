#include <Game.h>

#ifndef SHIPPING
#include <Magnum/ImGuiIntegration/Context.h>
#include <imgui.h>
#endif

namespace GDEEditor
{
    class Editor final : public GDE::Game
    {
    public:
        explicit Editor(const Arguments& arguments);

        void setupSystem() override;
        void registerComponent() const override;
        void setupScene() override;
        void drawEvent() override;

    private:
        ImGuiIntegration::Context _imguiContext;
    };
}
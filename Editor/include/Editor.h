#include <Game.h>
#ifndef SHIPPING
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
    };
}
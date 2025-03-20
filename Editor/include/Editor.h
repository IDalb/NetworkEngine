#pragma once

#include <Magnum/Math/Color.h>
#include <Magnum/Math/Time.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>

#ifndef SHIPPING
#include <Magnum/ImGuiIntegration/Context.hpp>
#endif
#include <Game.h>

#include "Magnum/ImGuiIntegration/Context.hpp"

namespace GDEEditor
{
    class Editor final : public GDE::Game
    {
    public:
        explicit Editor(const Arguments& arguments);

        void setupSystem() override;
        void registerComponent() const override;
        void setupScene() override;
        //void drawEvent() override;

       void viewportEvent(ViewportEvent& event) override;

    private:
        ImGuiIntegration::Context _imguiContext{ NoCreate };
        bool _showDemoWindow = true;
        bool _showAnotherWindow = false;
        Color4 _clearColor = 0x72909aff_rgbaf;
        Float _floatValue = 0.0f;
    };
}

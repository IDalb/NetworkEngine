#pragma once
#include "System/System.h"
#include "Component/DisplayComponent.h"
#include "std.h"
namespace GDE
{
    class DisplaySystem :
    public System
    {
    public:
        DisplaySystem() = default;
        DisplaySystem(const DisplaySystem&) = default;
        DisplaySystem(DisplaySystem&&) = default;
        DisplaySystem& operator=(const DisplaySystem&) = default;
        DisplaySystem& operator=(DisplaySystem&&) = default;

        ~DisplaySystem() = default;

        void setup();

        void iterate(const Timing& dt) override;

        void registerComponent(DisplayComponent* display_component);
        void removeComponent(DisplayComponent* display_component);

        static DisplaySystem& getInstance();
    private:
        std::unordered_set<DisplayComponent*> _displayComponents;
    };
}


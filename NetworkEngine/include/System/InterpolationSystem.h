#pragma once
#include "System/System.h"
#include "Component/InterpolationComponent.h"

namespace GDE
{
    class InterpolationSystem :
        public System
    {
        public:
            InterpolationSystem() = default;
            InterpolationSystem(const InterpolationSystem&) = default;
            InterpolationSystem(InterpolationSystem&&) = default;
            InterpolationSystem& operator=(const InterpolationSystem&) = default;
            InterpolationSystem& operator=(InterpolationSystem&&) = default;

            ~InterpolationSystem() = default;

            void iterate(const Timing& dt) override;

            void registerComponent(InterpolationComponent* logic_component);
            void removeComponent(InterpolationComponent* logic_component);

            static InterpolationSystem& getInstance();
            void setEnable(bool enable) { _enable = enable; }

        private:
            bool _enable = true;
            std::unordered_set<InterpolationComponent*> _interpolationComponents;
        
    };
}


#pragma once
#include "System/System.h"
#include "Component/AlternateLogicComponent.h"

namespace GDE
{
    class AlternateLogicSystem :
        public System
    {
        public:
            AlternateLogicSystem() { _exist = true; }
            AlternateLogicSystem(const AlternateLogicSystem&) = default;
            AlternateLogicSystem(AlternateLogicSystem&&) = default;
            AlternateLogicSystem& operator=(const AlternateLogicSystem&) = default;
            AlternateLogicSystem& operator=(AlternateLogicSystem&&) = default;

            ~AlternateLogicSystem() = default;

            void iterate(const Timing& dt) override;

            void registerComponent(AlternateLogicComponent* logic_component);
            void removeComponent(AlternateLogicComponent* logic_component);

            static AlternateLogicSystem& getInstance();
            inline static bool _exist = false;

            void setEnable(bool enable) { _enable = enable; }

        private:
            bool _enable = true;
            std::unordered_set<AlternateLogicComponent*> _logicComponents;
        
    };
}


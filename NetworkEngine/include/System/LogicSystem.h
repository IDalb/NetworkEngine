#pragma once
#include "System/System.h"
#include "Component/LogicComponent.h"

namespace GDE
{
    class LogicSystem :
        public System
    {
        public:
            LogicSystem() = default;
            LogicSystem(const LogicSystem&) = default;
            LogicSystem(LogicSystem&&) = default;
            LogicSystem& operator=(const LogicSystem&) = default;
            LogicSystem& operator=(LogicSystem&&) = default;

            ~LogicSystem() = default;

            void iterate(const Timing& dt) override;

            void registerComponent(LogicComponent* logic_component);
            void removeComponent(LogicComponent* logic_component);

            static LogicSystem& getInstance();
        private:
            std::unordered_set<LogicComponent*> _logicComponents;
        
    };
}


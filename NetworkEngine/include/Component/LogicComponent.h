#pragma once
#include "Component/Component.h"
#include "Timing.h"

namespace GDE
{
    class LogicComponent :
        public virtual Component
    {
    public :
        LogicComponent();
        ~LogicComponent();
        LogicComponent(const LogicComponent&) = default;
        LogicComponent& operator=(const LogicComponent&) = default;
        LogicComponent(LogicComponent&&) = default;
        LogicComponent& operator=(LogicComponent&&) = default;

        virtual void updateLogic(const Timing& timing) = 0;
    };
}


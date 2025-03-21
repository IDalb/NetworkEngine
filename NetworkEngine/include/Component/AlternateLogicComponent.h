#pragma once
#include "Component/Component.h"
#include "Timing.h"

namespace GDE
{
    class AlternateLogicComponent :
        public virtual Component
    {
    public :
        AlternateLogicComponent();
        ~AlternateLogicComponent();
        AlternateLogicComponent(const AlternateLogicComponent&) = default;
        AlternateLogicComponent& operator=(const AlternateLogicComponent&) = default;
        AlternateLogicComponent(AlternateLogicComponent&&) = default;
        AlternateLogicComponent& operator=(AlternateLogicComponent&&) = default;

        virtual void updateAlternateLogic(const Timing& timing) = 0;
    };
}


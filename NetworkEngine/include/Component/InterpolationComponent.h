#pragma once
#include "Component/Component.h"
#include "Timing.h"

namespace GDE
{
    class InterpolationComponent :
        public virtual Component
    {
    public :
        InterpolationComponent();
        ~InterpolationComponent();
        InterpolationComponent(const InterpolationComponent&) = default;
        InterpolationComponent& operator=(const InterpolationComponent&) = default;
        InterpolationComponent(InterpolationComponent&&) = default;
        InterpolationComponent& operator=(InterpolationComponent&&) = default;

        virtual void interpolate(const Timing& timing) = 0;
    };
}


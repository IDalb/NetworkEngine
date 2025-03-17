#pragma once
#include "Magnum/Magnum.h"

namespace GDE {

    class System {
    public:
        System() = default;
        System(const System&) = default;
        System(System&&) = default;
        System& operator=(const System&) = default;
        System& operator=(System&&) = default;

        virtual ~System() = default;

        virtual void iterate(const Magnum::Float dt_ms) {}
    };
}

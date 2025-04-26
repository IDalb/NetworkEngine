#pragma once
#include <vector>

#include "System.h"
#include "TypeDef.h"
#include "Timing.h"

namespace GDE
{
    class EntitySystem : public System {
    private:
        std::vector<EntityRef> _entitiesToRemove;

    public:
        EntitySystem() = default;
        EntitySystem(const EntitySystem&) = default;
        EntitySystem(EntitySystem&&) = default;
        EntitySystem& operator=(const EntitySystem&) = default;
        EntitySystem& operator=(EntitySystem&&) = default;
        ~EntitySystem() = default;

        const  std::vector<EntityRef>& getEntityToRemove() const { return _entitiesToRemove; }

        void iterate(const Timing&) override { _entitiesToRemove.clear(); }

        static EntitySystem& getInstance();
        static void remove(const EntityRef& entity);
    };
}

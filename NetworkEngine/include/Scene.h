#pragma once

#include <vector>
#include "TypeDef.h"

namespace GDE
{
    class Scene
    {
    public:
        static EntityRef createEntity(const Description& description, const EntityRef& parent);
        EntityRef addEntityToRoot(const Description& description);
        EntityRef findEntity(const std::string& name);

        Entity* findEntityWithTag(const std::string_view& tag);
        std::vector<Entity*>* findEntitiesWithTag(const std::string_view& tag);
        static void removeTagFromEntity(const EntityRef& entity, const std::string& tag);

        void load(const Description& description);
        void clear();
    };
}

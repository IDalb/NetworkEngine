#pragma once

#include "std.h"
#include "TypeDef.h"

namespace GDE
{
    namespace Scene
    {
        EntityRef createEntity(const Description& description, const EntityRef& parent);
        EntityRef addEntityToRoot(const Description& description);
        EntityRef findEntity(const std::string& name);

        Entity* findEntityWithTag(const std::string_view& tag);
        std::vector<Entity*>* findEntitiesWithTag(const std::string_view& tag);
        void removeTagFromEntity(const EntityRef& entity, const std::string& tag);

        Scene3D& getMagnumScene();

        void load(const Description& description);
        void clear();

        std::string serialize(uint32_t frame);
        void deserialize();
    }
}

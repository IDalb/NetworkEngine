#include "System/EntitySystem.h"
#include "Entity.h"
#include "Scene.h"

namespace GDE
{
    EntitySystem& EntitySystem::getInstance() {
        static EntitySystem instance;
        return instance;
    }

    void recursiveRemoveTag(const EntityRef& entity) {
        if (!entity->getTag().empty())
            Scene::removeTagFromEntity(entity, entity->getTag());

        for (auto& child : entity->getChildren())
            recursiveRemoveTag(child);
    }

    void EntitySystem::remove(const EntityRef& entity)
    {
        recursiveRemoveTag(entity);

        entity->setActive(false);
        getInstance()._entitiesToRemove.push_back(entity);
    }
}

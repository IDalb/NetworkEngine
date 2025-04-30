#include "Scene.h"
#include "Entity.h"
#include "Utils/MagnumScene.h"
#include "System/EntitySystem.h"
#include <map>
#include "Utils/LinkingContext.h"
#include "Game.h"
namespace GDE
{
    namespace Scene
    {
        inline constexpr int SIZE_BEGIN = sizeof(uint32_t);
        inline constexpr int NB_REMOVED_ENTITY_BEGIN = SIZE_BEGIN + sizeof(uint16_t);
        inline constexpr int  NB_ENTITY_BEGIN = NB_REMOVED_ENTITY_BEGIN + sizeof(uint16_t);
        inline constexpr int DATA_BEGIN = NB_ENTITY_BEGIN + sizeof(uint16_t);
        inline constexpr int ENTITY_ID_SIZE = sizeof(uint32_t);

        struct EntityDescription
        {
            std::map<std::string, Description> children;
            std::map<std::string, ComponentDescription> components;

            explicit EntityDescription(const Description& description)
            {
                for (auto& child : description["children"])
                    children.insert({ child["name"].as<std::string>(), child });

                for (auto& component : description["components"])
                    components.insert({ component.first.as<std::string>(), ComponentDescription(component.second) });
            }
        };

        EntityRef rootEntity()
        {
            static auto root = Entity::create();
            return root;
        }

        std::map<std::string, std::vector<Entity*>>& tagMap()
        {
            static std::map<std::string, std::vector<Entity*>> tagMap;
            return tagMap;
        }

        std::map<uint32_t, Entity*>& idMap()
        {
            static std::map<uint32_t, Entity*> idMap;
            return idMap;
        }

        EntityRef CreateEntityInternal(const EntityDescription& description, const EntityRef& parent, const std::string_view& name, const std::string& tag, uint32_t id)
        {
            EntityRef newEntity = Entity::create();
            if (id == 0)
            {
                static uint32_t entityIds = 1;
                newEntity->setId(entityIds);
                entityIds++;
            }
            else
            {
                newEntity->setId(id);
            }
            parent->addChild(std::string(name), newEntity);
            for (const auto& component : description.components)
                newEntity->addComponent(component.first, component.second);

            for (const auto& child : description.children)
                createEntity(child.second, newEntity);

            if (!tag.empty())
            {
                tagMap()[tag].push_back(newEntity.get());
                newEntity->setTag(tag);
            }
            newEntity->resolve();

            return newEntity;
        }

        EntityRef createEntity(const Description& description, const EntityRef& parent, uint32_t id)
        {
            std::string tag;
            if (description["tag"])
            {
                tag = description["tag"].as<std::string>();
            }
            return CreateEntityInternal(EntityDescription(description["description"]), parent, description["name"].as<std::string>(), tag, id);
        }

        EntityRef GDE::Scene::createEntity(const Description& description, const EntityRef& parent, std::string_view templateName, uint32_t id)
        {
            EntityRef newEntity = createEntity(description, parent, id);
            newEntity->setTemplateName(std::string(templateName));
            return newEntity;
        }

        EntityRef addEntityToRoot(const Description& description)
        {
            return createEntity(description, rootEntity());
        }

        void load(const Description& description)
        {
            for (const auto& entityDescription : description)
                addEntityToRoot(entityDescription);
        }

        void clear()
        {
            rootEntity()->clear();
            tagMap().clear();
        }

        uint16_t recursiveEntitySerialization(EntityRef entity, std::string& data)
        {
            uint16_t entityCount = 0;
            auto children = entity->getChildren();
            if (children.size() > 0)
            {
                for (auto& child : children)
                {
                    entityCount += recursiveEntitySerialization(child, data);
                }
            }
            std::string v = entity->serialize();
            if (v != "")
            {
                data += v;
                entityCount++;
            }
            return entityCount;
        }

        std::string serialize(uint32_t frame)
        {
            // penser au dÈpassement de capacitÅE
            std::string data;
            const std::vector<EntityRef>& removedEntity = EntitySystem::getInstance().getEntityToRemove();
            std::span<EntityRef> allEntity = Scene::rootEntity()->getChildren();
            uint16_t removedEntityCount = std::size(removedEntity);
            
            data.resize(sizeof(frame) + 3 * sizeof(uint16_t) + std::size(removedEntity) * sizeof(uint32_t)); // size of the data, number of actor, number of deleted actor
            memcpy(data.data(), &frame, sizeof(frame));
            memcpy(data.data() + sizeof(frame) + sizeof(uint16_t), &removedEntityCount, sizeof(frame));


            for (size_t i = 0; i < std::size(removedEntity); i++)
            {
                uint32_t id = removedEntity[i]->getId();
                memcpy(data.data() + i * sizeof(id), &id, sizeof(id));
            }

            uint16_t entityCount = 0;
            for (auto& entity : allEntity)
            {
                entityCount += recursiveEntitySerialization(entity, data);
            }
            memcpy(data.data() + sizeof(frame) + sizeof(uint16_t) + sizeof(removedEntityCount), &entityCount, sizeof(entityCount));

            uint16_t dataSize = data.length() - sizeof(frame);
            memcpy(data.data() + SIZE_BEGIN, &dataSize, sizeof(dataSize));

            return data;
        }

        void deserialize(char*& data)
        {

            uint32_t frameIndex;
            memcpy(&frameIndex, data, sizeof(frameIndex));
            // recover frame index
            uint16_t dataSize;
            memcpy(&dataSize, data + SIZE_BEGIN, sizeof(uint16_t));

            uint16_t removedEntityCount;
            memcpy(&removedEntityCount, data + NB_REMOVED_ENTITY_BEGIN, sizeof(uint16_t));
            uint16_t entityCount;
            memcpy(&entityCount, data + NB_ENTITY_BEGIN, sizeof(uint16_t));
            data = data + DATA_BEGIN;
            for (size_t i = 0; i < removedEntityCount; i++)
            {
                uint32_t entityId;
                memcpy(&entityId, data, sizeof(uint32_t));

                EntitySystem::getInstance().remove(Scene::getEntityFromId(entityId)->shared_from_this());
                data += sizeof(uint32_t);
            }
            uint16_t deserializedEntity = 0;
            while (deserializedEntity < entityCount)
            {
                uint32_t entityId;
                memcpy(&entityId, data, ENTITY_ID_SIZE);
                if (Scene::getEntityFromId(entityId) != nullptr)
                {
                    data += (ENTITY_ID_SIZE + sizeof(NetworkTemplateSize));
                    Scene::getEntityFromId(entityId)->deserialize(data, frameIndex);
                }
                else
                {
                    NetworkTemplateSize templateId;
                    memcpy(&templateId, data + ENTITY_ID_SIZE, sizeof(NetworkTemplateSize));

                    EntityRef newEntity = Scene::createEntity(Descr::load(Game::_app->getTemplatePath() + LinkingContext<NetworkTemplateSize>::getInstance().getTemplateFromId(templateId))[0], Scene::rootEntity(), LinkingContext<NetworkTemplateSize>::getInstance().getTemplateFromId(templateId), entityId);
                    data+= (ENTITY_ID_SIZE + sizeof(NetworkTemplateSize));
                    newEntity->deserialize(data, frameIndex);

                }
                deserializedEntity++;
            }
        }

        EntityRef FindEntityRecursive(const EntityRef& parent, const std::string_view& name)
        {
            auto found = parent->getChild(name.data());

            if (found != nullptr)
                return found;

            for (const auto& child : parent->getChildren())
            {
                found = FindEntityRecursive(child, name);
                if (found)
                    return found;
            }
            return {};
        }

        EntityRef findEntity(const std::string& name)
        {
            return FindEntityRecursive(rootEntity(), name);
        }

        void removeTagFromEntity(const EntityRef& entity, const std::string& tag)
        {
            auto& map = tagMap();
            auto& tagVector = map.at(tag);
            std::erase(tagVector, entity.get());

            if (tagVector.empty())
                map.erase(tag);
        }

        void removeEntityId(uint32_t id)
        {
            idMap().erase(id);
        }

        void addEntityId(uint32_t id, Entity* entity)
        {
            idMap().insert({ id, entity });
        }

        Entity* getEntityFromId(uint32_t id)
        {
            return idMap().contains(id) ? idMap().at(id) : nullptr;
        }

        Scene3D& getMagnumScene()
        {
            static MagnumScene scene;
            return scene.getScene();
        }

        Entity* findEntityWithTag(const std::string_view& tag)
        {
            if (const std::string str_tag(tag); tagMap().contains(str_tag))
                return tagMap().at(str_tag)[0];

            return nullptr;
        }

        std::vector<Entity*>* findEntitiesWithTag(const std::string_view& tag)
        {
            if (const std::string str_tag(tag); tagMap().contains(str_tag))
                return &tagMap().at(str_tag);

            return nullptr;
        }
    }
}

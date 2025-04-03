#include "Scene.h"
#include "Entity.h"
#include "Utils/MagnumScene.h"
#include <map>

namespace GDE
{
    namespace Scene
    {
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

        EntityRef CreateEntityInternal(const EntityDescription& description, const EntityRef& parent, const std::string_view& name, const std::string& tag)
        {
            EntityRef newEntity = Entity::create();
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

        EntityRef createEntity(const Description& description, const EntityRef& parent)
        {
            std::string tag;
            if (description["tag"])
            {
                tag = description["tag"].as<std::string>();
            }
            return CreateEntityInternal(EntityDescription(description["description"]), parent, description["name"].as<std::string>(), tag);
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

        std::string serialize(uint32_t frame)
        {
            // penser au dépassement de capacité
            std::string data;
            data.resize(sizeof(frame) + 3 * sizeof(uint16_t)); // size of the data, number of actor, number of deleted actor
            Scene::rootEntity()->getChildren();

            return data;
        }

        void deserialize()
        {
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

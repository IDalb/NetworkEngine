#pragma once
#include "std.h"

#include "TypeDef.h"
#include "Component/Component.h"

namespace GDE
{
    class Entity : public std::enable_shared_from_this<Entity> {
    private:
        bool _active{ true };
        std::string _tag{};
        std::string _name{};

        uint32_t _id{};

        std::weak_ptr<Entity> _parent;
        std::vector<EntityRef> _children;
        std::map<std::string, std::weak_ptr<Entity>> _childrenByName;
        std::map<std::string, std::unique_ptr<Component>> _components;

        static constexpr unsigned int COMPONENT_ID = sizeof(_id);
        static constexpr unsigned int SIZEOF_SIZE = sizeof(uint16_t);
    public:
        Entity();
        ~Entity();
        static EntityRef create() { return std::make_shared<Entity>(); }

        // Network thing to do to send a create request with the path
        // then send serialized data

        std::string serialize()
        {
            std::string data;
            data.resize(COMPONENT_ID + 1);
            memcpy(data.data(), &_id, sizeof(_id));
            uint8_t serializedComponentCount = 0;
            for (auto& pair : _components)
            {
                const std::string serializedComponent = pair.second->serialize();
                data += serializedComponent;
                if (serializedComponent != "")
                {
                    serializedComponentCount++;
                }
            }        
            memcpy(data.data() + COMPONENT_ID, &serializedComponentCount, sizeof(serializedComponentCount));
            if (serializedComponentCount == 0)
            {
                return "";
            }
            return data;
        }
        void deserialize(char*& data)
        {
            uint8_t serializedComponentCount;
            uint8_t deserializedComponentCount = 0;
            memcpy(&serializedComponentCount, data + COMPONENT_ID, sizeof(serializedComponentCount));

            data += (sizeof(COMPONENT_ID) + 1);
            while (serializedComponentCount != deserializedComponentCount)
            {
                uint32_t componentId;
                memcpy(&componentId, data, sizeof(componentId));
                _components.at(Component::getComponentName(componentId))->deserialize(data);
                deserializedComponentCount++;
            }
        }

        void resolve();
        void clear();

        bool active() const { return _active; }
        void setActive(const bool active) {
            _active = active;
            for (auto& child : _children) child->setActive(_active);
        }

        const std::string& getTag() const { return _tag; }
        const std::string& getName() const { return _name; }
        uint32_t getId() const { return _id; }

        void setTag(const std::string& tag) { _tag = tag; }
        void setName(const std::string& name) { _name = name; }
        void setId(uint32_t id) { _id = id; }

        EntityRef getParent() const { return _parent.lock(); }
        EntityRef getChild(const std::string& name) const;
        std::span<EntityRef> getChildren() { return _children; }

        void addChild(const std::string& name, const EntityRef& entity);
        void removeChild(EntityRef child);
        void removeChild(Entity& child) { removeChild(child.shared_from_this()); }

        Component* addComponent(const std::string& type, const ComponentDescription& initial_value);
        template<typename T> 
        T* addComponent(const typename T::init_values& initial_value)
        {
            auto result = _components.emplace(T::type, std::unique_ptr<T>(*this));
            auto iterator = result.first;
            T* component = dynamic_cast<T*>(iterator->second.get());
            component->setup(initial_value);
        }
        Component* getComponent(const std::string& type) const;
        template<typename T> T* getComponent() const
        {
            return dynamic_cast<T*>(getComponent(T::type)); 
        }

    };
}

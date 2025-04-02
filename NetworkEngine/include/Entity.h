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
        std::unordered_map<std::string, std::unique_ptr<Component>> _components;

        static constexpr unsigned int COMPONENT_ID = sizeof(_id);
        static constexpr unsigned int SIZEOF_SIZE = sizeof(uint16_t);
    public:
        ~Entity() { _components.clear(); _childrenByName.clear(); }
        static EntityRef create() { return std::make_shared<Entity>(); }

        // Network thing to do to send a create request with the path
        // then send serialized data

        std::string serialize()
        {
            std::string data;
            data.resize(COMPONENT_ID + SIZEOF_SIZE);
            memcpy(data.data(), &_id, sizeof(_id));
            for (auto& pair : _components)
            {
                data += pair.second->serialize();
            }
            return data;
        }
        void deserialize(std::span<char> data)
        {
            size_t componentDataBegin = COMPONENT_ID + SIZEOF_SIZE;
            for (auto& pair : _components)
            {
                size_t componentDataSize = 0;
                memcpy(&componentDataSize, data.data() + componentDataBegin + Component::SIZEOF_ID, Component::SIZEOF_SIZE);
                componentDataBegin += Component::SIZEOF_ID + Component::SIZEOF_SIZE;
                pair.second->deserialize(std::span<char>(data.data() + componentDataBegin, componentDataSize));
                componentDataBegin += componentDataSize;
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
        void setTag(const std::string& tag) { _tag = tag; }
        void setName(const std::string& name) { _name = name; }

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

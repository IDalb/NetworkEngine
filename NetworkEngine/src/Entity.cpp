#include "Entity.h"

#include "System/EntitySystem.h"

namespace GDE {
    void Entity::resolve() {
        for (auto& component : _components)
            component.second->resolve();
    }

    void Entity::clear() {
        for (const auto& child : _children)
            EntitySystem::remove(child);

        _childrenByName.clear();
        _children.clear();
        _components.clear();
    }


    EntityRef Entity::getChild(const std::string& name) const {
        if (_childrenByName.empty()) return nullptr;

        const auto child = _childrenByName.find(name);
        if (child == _childrenByName.end()) return nullptr;
        return child->second.lock();
    }

    void Entity::addChild(const std::string& name, const EntityRef& entity) {
        _children.push_back(entity);
        if (_childrenByName.contains(name))
        {
            int nameIndex = 1;
            while (_childrenByName.contains(name + std::to_string(nameIndex)))
            {
                nameIndex++;
            }
            _childrenByName[name + std::to_string(nameIndex)] = entity;

        }
        else
        {
            _childrenByName[name] = entity;
        }
        entity->_parent = weak_from_this();
    }

    void Entity::removeChild(EntityRef child) {
        _children.erase(std::remove(_children.begin(), _children.end(), child), _children.end());
        child->_parent.reset();
        EntitySystem::remove(child);
    }


    Component* Entity::addComponent(const std::string& type, const ComponentDescription& initial_value) {
        auto result = _components.emplace(type, Component::create(type, *this));
        auto iterator = result.first;
        auto* component = dynamic_cast<Component*>(iterator->second.get());
        component->setup(initial_value);
        return component;
    }



    Component* Entity::getComponent(const std::string& type) const {
        auto component = _components.find(type);

        if (component == _components.end())
            return nullptr;

        return component->second.get();
    }
}

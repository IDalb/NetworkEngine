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

    void Entity::addChild(std::string name, const EntityRef& entity) {
        _children.push_back(entity);
        _childrenByName[name] = entity;
        entity->_parent = weak_from_this();
    }

    void Entity::removeChild(EntityRef child) {
        _children.erase(std::remove(_children.begin(), _children.end(), child), _children.end());
        child->_parent.reset();
        EntitySystem::remove(child);
    }


    Component* Entity::addComponent(std::string type, const ComponentDescription& initial_value) {
        auto result = _components.emplace(type, Component::create(type, *this));
        auto iterator = result.first;
        auto* component = dynamic_cast<Component*>(iterator->second.get());
        component->setup(initial_value);
        return component;
    }

    template<typename T> T* Entity::addComponent(const typename T::init_values& initial_value) {
        auto result = _components.emplace(T::type, std::unique_ptr<T>(*this));
        auto iterator = result.first;
        T* component = dynamic_cast<T*>(iterator->second.get());
        component->setup(initial_value);
    }

    Component* Entity::getComponent(std::string type) const {
        auto component = _components.find(type);

        if (component == _components.end())
            return nullptr;

        return component->second.get();
    }

    template<typename T> T* Entity::getComponent() const { return dynamic_cast<T*>(getComponent(T::type)); }
}

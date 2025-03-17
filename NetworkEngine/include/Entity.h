#pragma once
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <span>

#include "TypeDef.h"
#include "Component/Component.h"

namespace GDE
{
    class Entity : public std::enable_shared_from_this<Entity> {
    private:
        bool _active{true};
        std::string _tag{};

        std::weak_ptr<Entity> _parent;
        std::vector<EntityRef> _children;
        std::map<std::string, std::weak_ptr<Entity>> _childrenByName;
        std::unordered_map<std::string, std::unique_ptr<Component>> _components;
    public:
        ~Entity() { _components.clear(); _childrenByName.clear(); }
        static EntityRef create() { return std::make_shared<Entity>(); }

        void resolve();
        void clear();

        bool active() const { return _active; }
        void setActive(const bool active) {
            _active = active;
            for (auto& child : _children) child->setActive(_active);
        }

        const std::string& getTag() const { return _tag; }
        void setTag(const std::string& tag) { _tag = tag; }

        EntityRef getParent() const { return _parent.lock(); }
        EntityRef getChild(const std::string& name) const;
        std::span<EntityRef> getChildren() { return _children; }

        void addChild(std::string name, const EntityRef& entity);
        void removeChild(EntityRef child);
        void removeChild(Entity& child) { removeChild(child.shared_from_this()); }

        Component* addComponent(std::string type, const ComponentDescription& initial_value);
        template<typename T> T* addComponent(const typename T::init_values& initial_value);
        Component* getComponent(std::string type) const;
        template<typename T> T* getComponent() const;
    };
}

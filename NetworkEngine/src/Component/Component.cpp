#include "Component/Component.h"

namespace GDE
{
    void Component::Register(std::string_view type, const CreateFunctionType &create_function) {
        _registry.emplace(type, create_function);
    }

    std::unique_ptr<Component> Component::create(const std::string_view type, Entity &parent) {
        const auto& create_function = _registry.at(static_cast<std::string>(type));
        return create_function(parent);
    }


}

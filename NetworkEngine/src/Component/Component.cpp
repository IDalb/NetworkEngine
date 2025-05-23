#include "Component/Component.h"
#include "Functions/Crc32.h"

namespace GDE
{
    void Component::Register(std::string_view type, const CreateFunctionType &create_function) {
        _registry.emplace(type, create_function);
        componentIdToName[Crc32::crc32(type.data())] = type;
    }

    std::unique_ptr<Component> Component::create(const std::string_view type, Entity &parent) {
        const auto& create_function = _registry.at(static_cast<std::string>(type));
        return create_function(parent);
    }


}

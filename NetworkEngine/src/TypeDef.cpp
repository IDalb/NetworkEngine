#include "TypeDef.h"

#include <yaml-cpp/yaml.h>

namespace GDE
{
    Description load(const std::string &path) {
        return YAML::LoadFile(path);
    }
}

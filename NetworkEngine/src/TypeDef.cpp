#include "TypeDef.h"

#include <yaml-cpp/yaml.h>

namespace GDE
{
    namespace Descr
    {
        Description load(const std::string& path)
        {
            return YAML::LoadFile(path);
        }
    }
}

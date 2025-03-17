#pragma once
#include <memory>
#include "yaml-cpp/yaml.h"

namespace YAML {
    class Node;
}

namespace GDE {
    class Entity;
    typedef std::shared_ptr<Entity> EntityRef;
    typedef YAML::Node Description;

    namespace Descr {
        Description load(const std::string& path);
    }

}

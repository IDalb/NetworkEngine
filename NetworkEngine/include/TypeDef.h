#pragma once
#include "std.h"

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

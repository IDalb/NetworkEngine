#pragma once
#include "std.h"
#include "Magnum/SceneGraph/Object.h"
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Scene.h>

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

    typedef Magnum::SceneGraph::Object<Magnum::SceneGraph::MatrixTransformation3D> Object3D;
    typedef Magnum::SceneGraph::Scene<Magnum::SceneGraph::MatrixTransformation3D> Scene3D;

}

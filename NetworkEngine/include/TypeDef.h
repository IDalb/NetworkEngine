#pragma once
#include "std.h"
#include "Magnum/SceneGraph/Object.h"
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Scene.h>

#include <enet6/enet.h>

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

    //enet redefinition
    typedef ENetAddress NetworkAddress;
    typedef ENetPeer NetworkConnection;
    typedef ENetHost NetworkHost;
    typedef enum _NetworkAddressType
    {
        NETWORK_ADDRESS_TYPE_ANY  = ENET_ADDRESS_TYPE_ANY,
        NETWORK_ADDRESS_TYPE_IPV4 = ENET_ADDRESS_TYPE_IPV4,
        NETWORK_ADDRESS_TYPE_IPV6 = ENET_ADDRESS_TYPE_IPV6
    }NetworkAddressType;
}

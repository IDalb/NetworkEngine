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
    typedef ENetPacket NetworkPacket;

    typedef enum _NetworkEventType
    {
        /** no event occurred within the specified time limit */
        NETWORK_EVENT_TYPE_NONE  = ENET_EVENT_TYPE_NONE,
        NETWORK_EVENT_TYPE_CONNECT  = ENET_EVENT_TYPE_CONNECT,
        NETWORK_EVENT_TYPE_DISCONNECT  = ENET_EVENT_TYPE_DISCONNECT,
        NETWORK_EVENT_TYPE_RECEIVE  = ENET_EVENT_TYPE_RECEIVE,
        NETWORK_EVENT_TYPE_DISCONNECT_TIMEOUT  = ENET_EVENT_TYPE_DISCONNECT_TIMEOUT,

    } NetworkEventType;

    typedef ENetEvent NetworkEvent;
}

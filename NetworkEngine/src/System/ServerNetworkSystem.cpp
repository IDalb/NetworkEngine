#pragma once
#include "System/ServerNetworkSystem.h"
#include "Functions/Network.h"
namespace GDE
{
    ServerNetworkSystem& ServerNetworkSystem::getInstance()
    {
        static ServerNetworkSystem serverSystem;
        return serverSystem;
    }

    ServerNetworkSystem::ServerNetworkSystem()
    {
        Network::initialize();
    }

    ServerNetworkSystem::~ServerNetworkSystem()
    {
        if (_host != nullptr)
        {
            destroyServer();
        }
        Network::clean();
    }

    void ServerNetworkSystem::iterate(const Timing& dt)
    {
    }
    void ServerNetworkSystem::createServer(size_t maxClient, uint16_t port, NetworkAddressType addressType)
    {
        Network::initServerAddress(_address, addressType, port);
        _host = Network::createHost(addressType, nullptr, maxClient);
    }
    void ServerNetworkSystem::destroyServer()
    {
        Network::destroyHost(_host);
        _host = nullptr;
    }

    void ServerNetworkSystem::receiveThread(ServerNetworkSystem& serverSystem)
    { /* Connect and user service */
        int eventStatus = 1;
        ENetEvent event;
        while (1)
        {
            eventStatus = enet_host_service(serverSystem._host, &event, 100);

            /* If we had some event that interested us */
            if (eventStatus > 0)
            {
                switch (event.type)
                {
                case ENET_EVENT_TYPE_CONNECT:
                {
                    //enet_address_get_host_ip(&event.peer->address, addressBuffer, ENET_ADDRESS_MAX_LENGTH);
                    //printf("(Server) We got a new connection from %s\n", addressBuffer);
                    printf("connection");
                    break;
                }

                case ENET_EVENT_TYPE_RECEIVE:
                    printf("(Server) Message from client : %s\n", event.packet->data);
                    /* Re-send the message to all clients */
                    enet_host_broadcast(serverSystem._host, 0, event.packet);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
                {
                    //enet_address_get_host_ip(&event.peer->address, addressBuffer, ENET_ADDRESS_MAX_LENGTH);
                    //printf("Client %s disconnected%s.\n", addressBuffer, (event.type == ENET_EVENT_TYPE_DISCONNECT_TIMEOUT) ? " (timeout)" : "");
                    printf("disconnection");
                    break;
                }
            }
        }
    }
}


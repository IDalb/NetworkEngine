#pragma once
#include "System/ServerNetworkSystem.h"
#include "Functions/Network.h"
#include "Scene.h"
#include "Enums/NetworkMessage.h"
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
        _loop = false;
        _receiveThread.join();
        if (_host != nullptr)
        {
            destroyServer();
        }
        Network::clean();
    }

    void ServerNetworkSystem::iterate(const Timing& dt)
    {
        std::string data;

        data.resize(1);
        NetworkMessage::NetworkMessage msgType = NetworkMessage::SNAPSHOT;

        memcpy(data.data(), &msgType, sizeof(msgType));
        data += GDE::Scene::serialize(static_cast<uint32_t>(dt._frame));
        
        Network::broadcast(data, _host, true);
    }

    void ServerNetworkSystem::createServer(size_t maxClient, uint16_t port, NetworkAddressType addressType)
    {
        Network::initServerAddress(_address, addressType, port);
        _host = Network::createHost(NetworkAddressType::NETWORK_ADDRESS_TYPE_ANY, &_address, maxClient);
        _receiveThread = std::thread(receiveThread, std::ref(*this));
    }

    void ServerNetworkSystem::destroyServer()
    {
        _loop = false;
        if(_receiveThread.joinable())
        {
            _receiveThread.join();
        }
        Network::destroyHost(_host);
        _host = nullptr;
    }

    void ServerNetworkSystem::receiveThread(ServerNetworkSystem& serverSystem)
    { /* Connect and user service */
        int eventStatus = 1;
        NetworkEvent event;
        while (serverSystem._loop)
        {
            eventStatus = Network::hostReceive(serverSystem._host, &event, 100);

            /* If we had some event that interested us */
            if (eventStatus > 0)
            {
                switch (event.type)
                {
                case NETWORK_EVENT_TYPE_CONNECT:
                {
                    //enet_address_get_host_ip(&event.peer->address, addressBuffer, ENET_ADDRESS_MAX_LENGTH);
                    //printf("(Server) We got a new connection from %s\n", addressBuffer);
                    printf("connection\n");
                    break;
                }

                case NETWORK_EVENT_TYPE_RECEIVE:
                {
                    std::string data(reinterpret_cast<const char*>(event.packet->data), event.packet->dataLength);
                    NetworkMessage::NetworkMessage msgType;
                    memcpy(&msgType, data.data(), sizeof(msgType));

                    switch (msgType)
                    {
                    case NetworkMessage::PING:
                    {
                        NetworkMessage::NetworkMessage pong = NetworkMessage::PONG;
                        memcpy(data.data(), &pong, sizeof(pong));
                        Network::send(data, event.peer);
                    }
                    break;
                    case NetworkMessage::PONG:
                        break;
                    default:
                        break;
                    }
                    enet_packet_destroy(event.packet);
                }
                    break;

                case NETWORK_EVENT_TYPE_DISCONNECT:
                case NETWORK_EVENT_TYPE_DISCONNECT_TIMEOUT:
                {
                    //enet_address_get_host_ip(&event.peer->address, addressBuffer, ENET_ADDRESS_MAX_LENGTH);
                    //printf("Client %s disconnected%s.\n", addressBuffer, (event.type == ENET_EVENT_TYPE_DISCONNECT_TIMEOUT) ? " (timeout)" : "");
                    printf("disconnection\n");
                    break;
                }
                }
            }


        }
    }
}


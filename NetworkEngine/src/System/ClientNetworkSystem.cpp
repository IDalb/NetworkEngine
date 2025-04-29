#pragma once
#include "System/ClientNetworkSystem.h"
#include "Functions/Network.h"
#include "Enums/NetworkMessage.h"
#include "Scene.h"
#include <iostream>
namespace GDE
{
	ClientNetworkSystem& ClientNetworkSystem::getInstance()
	{
		static ClientNetworkSystem clientSystem;
		return clientSystem;
	}

    void ClientNetworkSystem::ping()
    {
        std::string msg;
        NetworkMessage::NetworkMessage msgType = NetworkMessage::PING;
        std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
        msg.resize(1 + sizeof(now));

        memcpy(msg.data(), &msgType, sizeof(msgType));
        memcpy(msg.data() + sizeof(msgType), &now, sizeof(now));

        Network::send(msg, _server);
    }

    ClientNetworkSystem::ClientNetworkSystem()
    {
        Network::initialize();
    }

    ClientNetworkSystem::~ClientNetworkSystem()
    {
        if (_host != nullptr)
        {
            disconnect();
        }
        Network::clean();
    }

    void ClientNetworkSystem::iterate(const Timing& dt)
	{
        int dataIndex = 1;
        {
            std::lock_guard<std::mutex> lock(_dataLock);
            if (_writeToFirstArray)
            {
                dataIndex = 0;
            }
            _writeToFirstArray = !_writeToFirstArray;
        }
            
        ping();

        while (std::size(_serverData[dataIndex]) > 0)
        {
            char* data = _serverData[dataIndex].back().data() + 1;
            Scene::deserialize(data);
            _serverData[dataIndex].pop_back();
        }
        std::lock_guard<std::mutex> lock(_latencyLock);
        std::cout << "latency : " << _latency << std::endl;
	}

	void ClientNetworkSystem::connect(const std::string& ip, uint16_t port, NetworkAddressType addressType)
	{
        Network::initClientTargetAddress(_address, addressType, port, ip);
		_host = Network::createHost(static_cast<NetworkAddressType>(_address.type), nullptr, 1);
		_server = Network::connect(&_address, _host);
        _receiveThread = std::thread(receiveThread, std::ref(*this));
	}

	void ClientNetworkSystem::disconnect()
	{
        _loop = false;
        if (_receiveThread.joinable())
        {
            _receiveThread.join();
        }		Network::disconnect(_server);
		Network::destroyHost(_host);
        _host = nullptr;
	}

    void ClientNetworkSystem::receiveThread(ClientNetworkSystem& clientSystem)
    {
        int eventStatus;
        NetworkEvent event;

        while (clientSystem._loop)
        {
            eventStatus = Network::hostReceive(clientSystem._host, &event, 100);

            /* Inspect events */
            if (eventStatus > 0)
            {
                switch (event.type)
                {
                case NETWORK_EVENT_TYPE_CONNECT:
                    printf("(Client) Connected to server\n");
                    break;

                case NETWORK_EVENT_TYPE_RECEIVE:
                {
                    std::string data(reinterpret_cast<const char*>(event.packet->data), event.packet->dataLength);
                    NetworkMessage::NetworkMessage msgType;
                    memcpy(&msgType, data.data(), sizeof(msgType));

                    switch (msgType) 
                    {
                    case NetworkMessage::SNAPSHOT:
                    {
                        std::lock_guard<std::mutex> lock(clientSystem._dataLock);
                        int dataIndex = 1;
                        if (clientSystem._writeToFirstArray)
                        {
                            dataIndex = 0;
                        }
                        clientSystem._serverData[dataIndex].push_back(data);
                    }
                        break;
                    case NetworkMessage::PONG:
                    {
                        std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
                        std::chrono::high_resolution_clock::time_point before;
                        memcpy(&before, data.data() + sizeof(msgType), sizeof(before));
                        clientSystem._latencyBuffer[clientSystem._latencyBufferCurrentIndex] = duration_cast<std::chrono::milliseconds>(now - before).count();
                        clientSystem._latencyBufferCurrentIndex++;
                        if (clientSystem._latencyBufferCurrentIndex == LATENCY_BUFFER_SIZE)
                        {
                            clientSystem._latencyBufferCurrentIndex = 0;
                            uint32_t sum = 0;
                            for (size_t i = 0; i < LATENCY_BUFFER_SIZE; i++)
                            {
                                sum += clientSystem._latencyBuffer[i];
                            }
                            std::lock_guard<std::mutex> lock(clientSystem._latencyLock);
                            clientSystem._latency = static_cast<float>(sum) / LATENCY_BUFFER_SIZE;
                        }

                    }
                        break;
                    default:
                        break;
                    }
                    enet_packet_destroy(event.packet);
                }
                break;
                case NETWORK_EVENT_TYPE_DISCONNECT:
                    printf("(Client) Disconnected from server.\n");
                    clientSystem._loop = false;
                    return;

                case NETWORK_EVENT_TYPE_DISCONNECT_TIMEOUT:
                    printf("(Client) Disconnected from server (timed out).\n");
                    clientSystem._loop = false;
                    return;
                }
            }
            // TODO : Ping
            
        }
    }
}


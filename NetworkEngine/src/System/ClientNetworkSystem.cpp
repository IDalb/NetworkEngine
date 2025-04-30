#pragma once
#include "System/ClientNetworkSystem.h"
#include "Functions/Network.h"
#include "Enums/NetworkMessage.h"
#include "Scene.h"
#include <iostream>
#include "Functions/Serialization.h"
#include "System/InputSystem.h"
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

    void ClientNetworkSystem::SendInput()
    {
        std::string msg;
        NetworkMessage::NetworkMessage messageType = NetworkMessage::INPUT;
        msg.resize(sizeof(NetworkMessage::NetworkMessage) + sizeof(_id) + sizeof(uint32_t));
        memcpy(msg.data(), &messageType, sizeof(NetworkMessage::NetworkMessage));
        memcpy(msg.data() + sizeof(NetworkMessage::NetworkMessage), &_id, sizeof(_id));
        uint32_t inputFrame;
        {
            std::lock_guard<std::mutex> lock(_latencyLock);
            inputFrame = _ServerLastFrame + uint32_t(_latency / _serverFpsMs) + 3;
        }
        memcpy(msg.data() + sizeof(NetworkMessage::NetworkMessage)+ sizeof(_id), &inputFrame, sizeof(inputFrame));

        std::string inputKeys;
        inputKeys.resize(1);
        uint8_t keyCount = 0;
        for (auto& keyState : InputSystem::getInstance().keys_state_)
        {
            if (keyState.second.state == InputSystem::State::PRESSED || keyState.second.state == InputSystem::State::RELEASED)
            {
                keyCount++;
                uint16_t key = static_cast<uint16_t>(keyState.first);
                if (keyState.second.state == InputSystem::State::PRESSED)
                {
                    key |= (1 << 15);
                }
                std::string keyData;
                keyData.resize(sizeof(uint16_t));
                memcpy(keyData.data(), &key, sizeof(key));
                inputKeys += keyData;
            }
        }
        memcpy(inputKeys.data(), &keyCount, sizeof(keyCount));
        msg += inputKeys;

        std::string mouseButtons;
        mouseButtons.resize(1);
        uint8_t buttonCount = 0;
        for (auto& mouseState : InputSystem::getInstance().mouse_buttons_state_)
        {
            if (mouseState.second.state == InputSystem::State::PRESSED || mouseState.second.state == InputSystem::State::RELEASED)
            {
                keyCount++;
                uint8_t button = static_cast<uint16_t>(mouseState.first);
                if (mouseState.second.state == InputSystem::State::PRESSED)
                {
                    button &= (1 << 7);
                }
                std::string mouseData;
                mouseData.resize(sizeof(uint16_t));
                memcpy(mouseData.data(), &button, sizeof(button));
                mouseButtons += mouseData;
            }
        }
        memcpy(mouseButtons.data(), &buttonCount, sizeof(buttonCount));
        msg += mouseButtons;

        Magnum::Vector2i mousePos = InputSystem::getInstance().getScreenMousePosition();
        Magnum::Vector2 mouseVel= InputSystem::getInstance().getMouseVelocity();

        uint32_t netMousePos = Serialization::combineInt(mousePos.x(), mousePos.y());
        uint32_t netMouseVel = Serialization::combineFloat(Serialization::serializeFloat(mouseVel.x()), Serialization::serializeFloat(mouseVel.y()));
    
        std::string mouseData;
        mouseData.resize(2 * sizeof(uint32_t));

        memcpy(mouseData.data(), &netMousePos, sizeof(uint32_t));
        memcpy(mouseData.data() + sizeof(uint32_t), &netMouseVel, sizeof(uint32_t));

        msg += mouseData;

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
        _currentFrameMs += dt._dt;
        if (_currentFrameMs > _serverFpsMs)
        {
            _currentFrameMs -= _serverFpsMs;
            _ServerLastFrame++;
        }



        int dataIndex = 1;
        {
            std::lock_guard<std::mutex> lock(_dataLock);
            if (_writeToFirstArray)
            {
                dataIndex = 0;
            }
            _writeToFirstArray = !_writeToFirstArray;
        }
            

        while (std::size(_serverData[dataIndex]) > 0)
        {
            char* data = _serverData[dataIndex].back().data() + 1;
            Scene::deserialize(data);
            _serverData[dataIndex].pop_back();
        }

        ping();
        SendInput();
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
                    case NetworkMessage::CONNECTION:
                    {
                        uint32_t serverLastFrame;
                        memcpy(&serverLastFrame, data.data() + sizeof(NetworkMessage::NetworkMessage), sizeof(uint32_t));
                        clientSystem._ServerLastFrame = serverLastFrame;
                    }
                    break;
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
                                sum += (clientSystem._latencyBuffer[i] / 2.f);
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


#pragma once
#include "System/ServerNetworkSystem.h"
#include "Functions/Network.h"
#include "Scene.h"
#include "Enums/NetworkMessage.h"
#include "System/ServerInputSystem.h"
#include "Functions/Serialization.h"
#include "Game.h"
#include <cpr/cpr.h>
#include "GlobalConstants.h"
#include <iostream>
#include "Utils/json.hpp"
namespace GDE
{


    ServerNetworkSystem& ServerNetworkSystem::getInstance()
    {
        static ServerNetworkSystem serverSystem;
        return serverSystem;
    }

    void ServerNetworkSystem::disconnectEverybody()
    {
        Network::disconnectEverybody(_host);
        _gameIdToNetId.clear();
        Scene::clear();
        Game::_app->setupScene();
        _gameStarted = false;
        ServerInputSystem::getInstance().clear();
        deleteFromWebApi();
        registerToWebApi();
    }

    void ServerNetworkSystem::registerToWebApi()
    {
        std::ostringstream request;

        // Get data
        request << constants::WEB_API_URL
            << "matches/connect";

        // Build proper JSON body
        std::ostringstream request_body;
        request_body << "{"
            << R"("ip": ")" << _serverIp << R"(",)"
            << R"("port": ")" << _serverPort << R"(",)"
            << R"("currentPlayers": [0])"
            << "}";

        // Send POST with proper headers
        cpr::Response rJoinServer = cpr::Post(
            cpr::Url{ request.str() },
            cpr::Header{ {"Content-Type", "application/json"} },  // Fix 415
            cpr::Body{ request_body.str() }
        );

        if (rJoinServer.status_code != 201) {
            std::cout << "Error " << rJoinServer.status_code << " when trying to connect the server" << std::endl;
        }
        else
        {
            nlohmann::json r_data = nlohmann::json::parse(rJoinServer.text);
            _apiId = r_data["id"];
        }
    }

    void ServerNetworkSystem::deleteFromWebApi()
    {
        std::ostringstream request;

        // Get data
        request << constants::WEB_API_URL
            << "matches/disconnect/" << _apiId;

     
        // Send POST with proper headers
        cpr::Response rJoinServer = cpr::Delete(
            cpr::Url{ request.str() },
            cpr::Header{ {"Content-Type", "application/json"} }
        );

        if (rJoinServer.status_code != 204) {
            std::cout << "Error " << rJoinServer.status_code << " when trying to connect the server" << std::endl;
        }
    }

    void ServerNetworkSystem::startGame()
    {
        std::string msg;
        msg.resize(1);
        NetworkMessage::NetworkMessage msgType = NetworkMessage::START;

        memcpy(msg.data(), &msgType, sizeof(msgType));
        Network::broadcast(msg, _host, true);
    }

    ServerNetworkSystem::ServerNetworkSystem()
    {
        Network::initialize();
    }

    ServerNetworkSystem::~ServerNetworkSystem()
    {
        deleteFromWebApi();
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
        bool serialize;
        {
            std::lock_guard<std::mutex> lock(_gameIdToNetIdLock);
            serialize = _gameIdToNetId.size() == _playerRequirement;
        }
        if (serialize)
        {
            if (!_gameStarted)
            {
                startGame();
                _gameStarted = true;
            }
            _lastFrame = dt._frame;
            std::string data;

            data.resize(1);
            NetworkMessage::NetworkMessage msgType = NetworkMessage::SNAPSHOT;

            memcpy(data.data(), &msgType, sizeof(msgType));
            data += GDE::Scene::serialize(static_cast<uint32_t>(dt._frame));

            //static int test = 0;
            //if(test == 20)
            {
                Network::broadcast(data, _host, true);
                //test = 0;
            }
            //test++;
            int dataIndex = 1;
            {
                std::lock_guard<std::mutex> lock(_inputLock);
                if (_writeToFirstArray)
                {
                    dataIndex = 0;
                }
                _writeToFirstArray = !_writeToFirstArray;
            }


            auto& input = ServerInputSystem::getInstance();
            while (std::size(_clientInput[dataIndex]) > 0)
            {
                char* inputData = _clientInput[dataIndex].back().data();

                uint32_t playerId, inputFrame;
                memcpy(&playerId, inputData + sizeof(NetworkMessage::NetworkMessage), sizeof(playerId));
                memcpy(&inputFrame, inputData + sizeof(NetworkMessage::NetworkMessage) + sizeof(uint32_t), sizeof(inputFrame));

                inputData += sizeof(NetworkMessage::NetworkMessage) + 2 * sizeof(uint32_t);
                uint8_t keyCount, mouseButtonCount;
                memcpy(&keyCount, inputData, sizeof(keyCount));
                inputData += sizeof(keyCount);
                for (uint8_t i = 0; i < keyCount; i++)
                {
                    uint16_t key;
                    memcpy(&key, inputData, sizeof(key));
                    inputData += sizeof(key);

                    bool value = (key >> 15);
                    // clear msb
                    key &= ~(1 << 15);

                    input.setKeyState(Key::Key(key), value, playerId, inputFrame);
                }

                memcpy(&mouseButtonCount, inputData, sizeof(mouseButtonCount));
                inputData += sizeof(mouseButtonCount);
                for (uint8_t i = 0; i < mouseButtonCount; i++)
                {
                    uint8_t button;
                    memcpy(&button, inputData, sizeof(button));
                    inputData += sizeof(button);

                    bool value = (button >> 7);
                    // clear msb
                    button &= ~(1 << 7);

                    input.setMouseButtonState(Mouse::Button(button), value, playerId, inputFrame);
                }

                uint32_t mousePos, mouseVel;
                memcpy(&mousePos, inputData, sizeof(mousePos));
                memcpy(&mouseVel, inputData + sizeof(mousePos), sizeof(mouseVel));

                auto [px, py] = Serialization::separateInt(mousePos);
                input.setMousePos(Magnum::Vector2i(px, py), playerId, inputFrame);

                auto [vx, vy] = Serialization::separateFloat(mouseVel);
                input.setMouseVelocity(Magnum::Vector2(Serialization::deserializeFloat(vx), Serialization::deserializeFloat(vy)), playerId, inputFrame);


                _clientInput[dataIndex].pop_back();
            }
        }
    }

    void ServerNetworkSystem::createServer(const GDE::Description& config, NetworkAddressType addressType)
    {
        std::string ip = config["server_ip"].as<std::string>();
        uint16_t port = config["server_port"].as<uint16_t>();

        Network::initServerAddress(_address, addressType, port);
        _host = Network::createHost(NetworkAddressType::NETWORK_ADDRESS_TYPE_ANY, &_address, _playerRequirement);
        _receiveThread = std::thread(receiveThread, std::ref(*this));
        
        _serverPort = port;
        _serverIp = ip;

        registerToWebApi();
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
                    std::string data;
                    data.resize(sizeof(NetworkMessage::NetworkMessage) + 2 * sizeof(uint32_t));
                    uint32_t lastFrame = serverSystem._lastFrame;
                    NetworkMessage::NetworkMessage connection = NetworkMessage::CONNECTION;

                    memcpy(data.data(), &connection, sizeof(connection));
                    memcpy(data.data() + sizeof(NetworkMessage::NetworkMessage), &lastFrame, sizeof(lastFrame));
                    uint32_t gameId;
                    {
                        std::lock_guard<std::mutex> lock(serverSystem._gameIdToNetIdLock);
                        gameId = serverSystem._gameIdToNetId.size();
                        serverSystem._gameIdToNetId.insert({ gameId, 0 });
                    }

                    memcpy(data.data() + sizeof(NetworkMessage::NetworkMessage) + sizeof(lastFrame), &gameId, sizeof(gameId));
                    Network::send(data, event.peer);
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
                    case NetworkMessage::CONNECTION:
                    {
                        uint32_t gameId, netId;
                        memcpy(&gameId, data.data() + +sizeof(NetworkMessage::NetworkMessage), sizeof(gameId));
                        memcpy(&netId, data.data() + +sizeof(NetworkMessage::NetworkMessage) + sizeof(gameId), sizeof(netId));
                        {
                            std::lock_guard<std::mutex> lock(serverSystem._gameIdToNetIdLock);
                            serverSystem._gameIdToNetId.at(gameId) = netId;
                        }
                    }
                    break;
                    case NetworkMessage::INPUT:
                    {
                        std::lock_guard<std::mutex> lock(serverSystem._inputLock);
                        int dataIndex = 1;
                        if (serverSystem._writeToFirstArray)
                        {
                            dataIndex = 0;
                        }
                        serverSystem._clientInput[dataIndex].push_back(data);
                    }
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


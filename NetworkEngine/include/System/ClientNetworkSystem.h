#pragma once
#include "System/System.h"
#include "TypeDef.h"
#include "Component/LogicComponent.h"
#include <enet6/enet.h>
namespace GDE
{
    class ClientNetworkSystem :
        public System
    {
        public:
            ClientNetworkSystem();
            ClientNetworkSystem(const ClientNetworkSystem&) = default;
            ClientNetworkSystem(ClientNetworkSystem&&) = default;
            ClientNetworkSystem& operator=(const ClientNetworkSystem&) = default;
            ClientNetworkSystem& operator=(ClientNetworkSystem&&) = default;

            ~ClientNetworkSystem();

            void iterate(const Timing& dt) override;

            void connect(const std::string& ip, uint16_t port, NetworkAddressType addressType);
            void disconnect();

            static void receiveThread(ClientNetworkSystem& clientSystem);

            static ClientNetworkSystem& getInstance();

            std::atomic_uint32_t _id = 0;
        private:
            NetworkAddress _address;
            NetworkConnection* _server;
            NetworkHost* _host;

            std::array<std::vector<std::string>, 2> _serverData;
            bool _writeToFirstArray = true;
            std::mutex _dataLock;
            std::atomic_bool _loop = true;
            std::thread _receiveThread;

            std::atomic_uint32_t _ServerLastFrame = 0;
            const float _serverFpsMs = ((1 / 60.f) * 1000);
            float _currentFrameMs = 0;

            static constexpr int LATENCY_BUFFER_SIZE = 60;
            std::array<uint32_t, LATENCY_BUFFER_SIZE> _latencyBuffer;
            size_t _latencyBufferCurrentIndex = 0;
            std::mutex _latencyLock;
            float _latency = 0;

            void ping();
            void SendInput();
    };
}


#pragma once
#include "System/System.h"
#include "Component/LogicComponent.h"

namespace GDE
{
    class ServerNetworkSystem :
        public System
    {
        public:
            ServerNetworkSystem();
            ServerNetworkSystem(const ServerNetworkSystem&) = default;
            ServerNetworkSystem(ServerNetworkSystem&&) = default;
            ServerNetworkSystem& operator=(const ServerNetworkSystem&) = default;
            ServerNetworkSystem& operator=(ServerNetworkSystem&&) = default;

            ~ServerNetworkSystem();

            void iterate(const Timing& dt) override;

            void createServer(size_t maxClient, uint16_t port, NetworkAddressType addressType);
            void destroyServer();

            static void receiveThread(ServerNetworkSystem& serverSystem);

            static ServerNetworkSystem& getInstance();

        private:
            NetworkAddress _address;
            NetworkHost* _host = nullptr;;

            std::array<std::vector<std::string>, 2> _clientInput;
            bool _writeToFirstArray = true;
            std::mutex _inputLock;

            std::thread _receiveThread;

            std::atomic_uint32_t _lastFrame = 0;
            std::atomic_bool _loop = true;
    };
}


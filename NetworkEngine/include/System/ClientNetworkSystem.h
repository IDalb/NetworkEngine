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
            ClientNetworkSystem() = default;
            ClientNetworkSystem(const ClientNetworkSystem&) = default;
            ClientNetworkSystem(ClientNetworkSystem&&) = default;
            ClientNetworkSystem& operator=(const ClientNetworkSystem&) = default;
            ClientNetworkSystem& operator=(ClientNetworkSystem&&) = default;

            ~ClientNetworkSystem() = default;

            void iterate(const Timing& dt) override;

            void connect(const std::string& ip, uint16_t port, NetworkAddressType addressType);
            void disconnect();

            static void receiveThread(ClientNetworkSystem& clientSystem);

            static ClientNetworkSystem& getInstance();

        private:
            NetworkAddress _address;
            NetworkConnection* _server;
            NetworkHost* _host;

            std::array<std::vector<std::string>, 2> _serverData;
            std::atomic_bool _writeToFirstArray = true;
            uint32_t _lastFrame = 0;
    };
}


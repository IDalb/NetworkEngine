#pragma once
#include "System/System.h"
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

            void connect();

            static void receiveThread();

            static ClientNetworkSystem& getInstance();

        private:
    };
}


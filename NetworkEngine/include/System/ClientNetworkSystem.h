#pragma once
#include "System/System.h"
#include "Component/LogicComponent.h"

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

            //void registerComponent(LogicComponent* logic_component);
            //void removeComponent(LogicComponent* logic_component);

            static ClientNetworkSystem& getInstance();

        private:
        
    };
}


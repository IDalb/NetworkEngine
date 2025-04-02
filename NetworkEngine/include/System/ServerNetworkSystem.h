#pragma once
#include "System/System.h"
#include "Component/LogicComponent.h"

namespace GDE
{
    class ServerNetworkSystem :
        public System
    {
        public:
            ServerNetworkSystem() = default;
            ServerNetworkSystem(const ServerNetworkSystem&) = default;
            ServerNetworkSystem(ServerNetworkSystem&&) = default;
            ServerNetworkSystem& operator=(const ServerNetworkSystem&) = default;
            ServerNetworkSystem& operator=(ServerNetworkSystem&&) = default;

            ~ServerNetworkSystem() = default;

            void iterate(const Timing& dt) override;

            //void registerComponent(LogicComponent* logic_component);
            //void removeComponent(LogicComponent* logic_component);

            static ServerNetworkSystem& getInstance();

        private:
        
    };
}


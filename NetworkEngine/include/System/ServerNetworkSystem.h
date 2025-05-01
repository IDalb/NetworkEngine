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

            void createServer(const GDE::Description& config, NetworkAddressType addressType);
            void destroyServer();

            static void receiveThread(ServerNetworkSystem& serverSystem);

            static ServerNetworkSystem& getInstance();

            std::map<uint32_t, uint32_t> _gameIdToNetId;

            void setPlayerRequirement(int count) { _playerRequirement = count; }
            void disconnectEverybody();
            
            const std::string& getServerIp() const { return _serverIp; }
            uint16_t getServerPort() const { return _serverPort; }

            void registerToWebApi();
            void deleteFromWebApi();
        private:
            NetworkAddress _address;
            NetworkHost* _host = nullptr;;

            std::array<std::vector<std::string>, 2> _clientInput;
            bool _writeToFirstArray = true;
            std::mutex _inputLock;
            std::mutex _gameIdToNetIdLock;

            std::thread _receiveThread;

            std::atomic_uint32_t _lastFrame = 0;
            std::atomic_bool _loop = true;

            int _playerRequirement = 4;
            bool _gameStarted = false;

            void startGame();

            std::string _serverIp = "";
            uint16_t _serverPort = 0;

            int _apiId = 0;
    };
}


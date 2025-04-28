#pragma once
#include "System/ClientNetworkSystem.h"
#include "Functions/Network.h"

namespace GDE
{
	ClientNetworkSystem& ClientNetworkSystem::getInstance()
	{
		static ClientNetworkSystem clientSystem;
		return clientSystem;
	}

	void ClientNetworkSystem::iterate(const Timing& dt)
	{
	}

	void ClientNetworkSystem::connect(const std::string& ip, uint16_t port, NetworkAddressType addressType)
	{
		_host = Network::createHost(addressType, nullptr, 1);
		_server = Network::connect(&_address, _host, ip, port);
	}

	void ClientNetworkSystem::disconnect()
	{
		Network::disconnect(_server);
		Network::destroyHost(_host);
	}

    void ClientNetworkSystem::receiveThread(ClientNetworkSystem& clientSystem)
    {
        bool running = true;
        int eventStatus;
        ENetEvent event;

        while (running)
        {
            eventStatus = enet_host_service(clientSystem._host, &event, 100);

            /* Inspect events */
            if (eventStatus > 0)
            {
                switch (event.type)
                {
                case ENET_EVENT_TYPE_CONNECT:
                    printf("(Client) Connected to server");
                    break;

                case ENET_EVENT_TYPE_RECEIVE:
                    printf("(Client) Message from server: %s\n", event.packet->data);
                    enet_packet_destroy(event.packet);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    printf("(Client) Disconnected from server.\n");
                    running = false;
                    return;

                case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
                    printf("(Client) Disconnected from server (timed out).\n");
                    running = false;
                    return;
                }
            }
            // TODO : Ping
            
            //else if (clientSystem._server->state == ENET_PEER_STATE_CONNECTED)
            //{
            //    /* Prompt some message to send to the server, be quick to prevent timeout (TODO: Read asynchronously) */
            //    printf("> ");
            //    fgets(message, sizeof(message), stdin);
            //
            //    if (strlen(message) > 0 && strcmp(message, "\n") != 0)
            //    {
            //        /* Build a packet passing our bytes, length and flags (reliable means this packet will be resent if lost) */
            //        ENetPacket* packet = enet_packet_create(message, strlen(message) + 1, ENET_PACKET_FLAG_RELIABLE);
            //        /* Send the packet to the server on channel 0 */
            //        enet_peer_send(serverPeer, 0, packet);
            //    }
            //    else
            //    {
            //        running = 0;
            //        enet_peer_disconnect_now(serverPeer, 0);
            //    }
            //}
        }
    }
}


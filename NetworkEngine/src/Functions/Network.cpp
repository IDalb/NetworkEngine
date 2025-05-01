#include "Functions/Network.h"

namespace GDE
{
	namespace Network
	{
		void initialize()
		{
			/* Initialize enet6 */
			if (enet_initialize() != 0)
			{
				printf("An error occured while initializing ENet6.\n");
			}
			atexit(enet_deinitialize);
		}

		void clean()
		{
			//atexit(enet_deinitialize);
		}

		void send(const std::string& data, NetworkConnection* target, bool reliable)
		{
			uint32_t flags = 0;
			if (reliable)
			{
				flags = ENET_PACKET_FLAG_RELIABLE;
			}
			ENetPacket* packet = enet_packet_create(data.c_str(), data.length() + 1, flags);
			/* Send the packet to the server on channel 0 */
			enet_peer_send(target, 0, packet);
		}

		void broadcast(const std::string& data, NetworkHost* server, bool reliable)
		{
			uint32_t flags = 0;
			if (reliable)
			{
				flags = ENET_PACKET_FLAG_RELIABLE;
			}
			ENetPacket* packet = enet_packet_create(data.c_str(), data.length() + 1, flags);
			enet_host_broadcast(server, 0, packet);
		}

		void initServerAddress(NetworkAddress& address, NetworkAddressType AddressType, size_t port)
		{
			enet_address_build_any(&address, static_cast<ENetAddressType>(AddressType));
			address.port = port;
		}

		void initClientTargetAddress(NetworkAddress& address, NetworkAddressType AddressType, size_t port, const std::string& ip)
		{
			enet_address_set_host(&address, static_cast<ENetAddressType>(AddressType), ip.c_str());
			address.port = port;
		}

		NetworkHost* createHost(NetworkAddressType AddressType, NetworkAddress* address, size_t maxClient)
		{
			NetworkHost* host = enet_host_create(
				static_cast<ENetAddressType>(AddressType),
				address,  /* the address to bind to */
				maxClient,       /* max clients */
				2,        /* max channels per connection */
				0,        /* downstream bandwidth (0 = unlimited) */
				0         /* upstream bandwidth (0 = unlimited) */
			);
			return host;
		}

		void destroyHost(NetworkHost* host)
		{
			enet_host_destroy(host);
		}

		int hostReceive(NetworkHost* host, NetworkEvent* event, uint32_t ms_delay)
		{
			return enet_host_service(host, event, 100);

		}

		NetworkConnection* connect(NetworkAddress* address, NetworkHost* host)
		{
			// Initiate the connection
			NetworkConnection* peer = enet_host_connect(host, address, 2, 0);
			if (peer == nullptr) {
				printf("No available peers for initiating an ENet connection.\n");
			}
			return peer;
		}

		void disconnect(NetworkConnection* peer)
		{
			enet_peer_disconnect(peer, 0);
		}
		void disconnectEverybody(NetworkHost* host)
		{
			for (size_t i = 0; i < host->peerCount; ++i)
			{
				ENetPeer* peer = &host->peers[i];
				if (peer->state == ENET_PEER_STATE_CONNECTED)
				{
					enet_peer_disconnect(peer, 0); // Clean disconnect (with flush and notification)
				}
			}
		}
	}
}
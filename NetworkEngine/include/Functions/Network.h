#pragma once
#include "TypeDef.h"
#include <enet6/enet.h>

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
			atexit(enet_deinitialize);
		}

		void initServerAddress(NetworkAddress& address, NetworkAddressType AddressType, size_t port)
		{
			enet_address_build_any(&address, static_cast<ENetAddressType>(AddressType));
			address.port = port;
		}

		void initClientAddress()
		{

		}

		NetworkHost* createHost(NetworkAddressType AddressType,  NetworkAddress* address, size_t maxClient)
		{
			NetworkHost* host = enet_host_create(
				static_cast<ENetAddressType>(AddressType),
				address,  /* the address to bind to */
				maxClient,       /* max clients */
				2,        /* max channels per connection */
				0,        /* downstream bandwidth (0 = unlimited) */
				0         /* upstream bandwidth (0 = unlimited) */
			);
		}

		void destroyHost(NetworkHost* host)
		{
			enet_host_destroy(host);
		}

		NetworkConnection* connect(NetworkAddress* address, NetworkHost* host,std::string_view ip, uint16_t port)
		{
			enet_address_set_host(address, ENET_ADDRESS_TYPE_ANY, ip.data());
			address->port = port;

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
	}
}
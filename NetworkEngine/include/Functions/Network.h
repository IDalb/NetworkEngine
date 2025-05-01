#pragma once
#include "TypeDef.h"
#include <enet6/enet.h>

namespace GDE
{
	namespace Network
	{
		void initialize();

		void clean();

		void send(const std::string& data, NetworkConnection* target, bool reliable = false);

		void broadcast(const std::string& data, NetworkHost* server, bool reliable = false);

		void initServerAddress(NetworkAddress& address, NetworkAddressType AddressType, size_t port);
		void initClientTargetAddress(NetworkAddress& address, NetworkAddressType AddressType, size_t port, const std::string& ip);

		NetworkHost* createHost(NetworkAddressType AddressType, NetworkAddress* address, size_t maxClient);

		void destroyHost(NetworkHost* host);

		int hostReceive(NetworkHost* host, NetworkEvent* event, uint32_t ms_delay);

		NetworkConnection* connect(NetworkAddress* address, NetworkHost* host);

		void disconnect(NetworkConnection* peer);
		void disconnectEverybody(NetworkHost* host);
	}
}
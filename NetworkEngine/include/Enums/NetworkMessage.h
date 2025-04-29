#pragma once
#include "std.h"
namespace GDE
{
	namespace NetworkMessage
	{
		enum NetworkMessage : uint8_t
		{
			SNAPSHOT, PING, PONG, INPUT
		};
	}
}
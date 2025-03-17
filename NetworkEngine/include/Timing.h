#pragma once
#include "std.h"
#include "Magnum/Magnum.h"

namespace GDE
{
	struct Timing
	{
		Magnum::Float dt_;
		unsigned int frame_;
	};
}
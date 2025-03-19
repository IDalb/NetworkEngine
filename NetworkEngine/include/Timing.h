#pragma once
#include "std.h"
#include "Magnum/Magnum.h"

namespace GDE
{
	struct Timing
	{
		Magnum::Float _dt;
		unsigned int _frame;
	};
}
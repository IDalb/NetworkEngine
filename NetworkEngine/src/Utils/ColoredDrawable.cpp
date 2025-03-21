#include "Utils/ColoredDrawable.h"
#include "System/DisplaySystem.h"
namespace GDE
{
	ColoredDrawable::~ColoredDrawable()
	{
		DisplaySystem::getInstance().getDrawable().remove(*this);
	}
}
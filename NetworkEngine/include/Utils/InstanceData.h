#pragma once 
#include "Magnum/Math/Matrix.h"
#include "Magnum/Math/Matrix4.h"
#include "Magnum/Math/Color.h"

namespace GDE
{
	struct InstanceData
	{
		Magnum::Matrix4 transformationMatrix;
		Magnum::Matrix3x3 normalMatrix;
		Magnum::Color3 color;
	};
}
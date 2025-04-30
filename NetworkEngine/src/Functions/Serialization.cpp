#include "Functions/Serialization.h"
#include <glm/gtc/quaternion.hpp>
#include <Magnum/Math/Vector4.h>
#include <cmath>
#include <iostream>
namespace GDE
{
	namespace Serialization
	{		
		serialized_float serializeFloat(float value, int factor)
		{
			return static_cast<serialized_float>(value * factor + 32767) & FLOAT_BIT_MASK;
		}

		serialized_float serializeFloatQuaternion(float value, int factor)
		{
			const serialized_float out = static_cast<serialized_float>(value * factor + 707/*minimal value*/);
			return static_cast<serialized_float>(std::round((value + 1.0f) * 511.5f));
			if (out > QUATERNION_BIT_MASK)
			{
				return QUATERNION_BIT_MASK;
			}
			return out & QUATERNION_BIT_MASK;
		}

		float deserializeFloat(serialized_float value, int factor)
		{
			return (static_cast<float>(value) - 32767) / factor;
		}

		float deserializeFloatQuaternion(serialized_float value, int factor)
		{
			return (static_cast<float>(value) / 511.5f) - 1.f;
			return (static_cast<float>(value) - 707 /*minimal value*/) / factor;
		}

		serialized_quaternion serializeQuaternion(const glm::quat& quat)
		{
			Magnum::Vector4 xyzw = {quat.x, quat.y , quat.z , quat.w };
			serialized_quaternion out = 0;
			int serializedValue = 0;
			float max = xyzw.max();
			for (uint32_t i = 0; i < 4; i++)
			{
				float v = *(xyzw.data() + i);
				if (v == max)
				{
					out |= (i << 30);
				}
				else
				{
					float tmp = serializeFloatQuaternion(v);
					out = out | (serializeFloatQuaternion(v) << serializedValue * 10);
					serializedValue++;
				}
			}
			glm::quat test = deserializeQuaternion(out);
			Magnum::Vector4 xyzw2 = { test.x, test.y , test.z , test.w };

			std::cout << "max index: " << max<<std::endl;
			std::cout << "ref x :"<< xyzw.x() <<" y :"<< xyzw.y() <<" z :"<< xyzw.z() <<" w :"<< xyzw.w() <<std::endl;
			std::cout << "new x :"<< xyzw2.x() <<" y :"<< xyzw2.y() <<" z :"<< xyzw2.z() <<" w :"<< xyzw2.w() <<std::endl;

			return out;
		}
		glm::quat deserializeQuaternion(serialized_quaternion quat)
		{
			serialized_quaternion quatCpy = quat;
			uint32_t missingIndex = quat >> 30;
			std::array<float, 4> xyzw;
			float* missingValueAddr = nullptr;
			float missingValue = 1;
			constexpr uint32_t MASK = createBitMask<10>();
			for (uint32_t i = 0; i < 4; i++)
			{
				uint32_t index = 3 - i;
				if (i == missingIndex)
				{
					missingValueAddr = &(xyzw[i]);
				}
				else
				{
					uint32_t tmp = quatCpy & MASK;
					xyzw[i] = deserializeFloatQuaternion(quatCpy & MASK, 1000);
					quatCpy >>= 10;
					missingValue -= (xyzw[i] * xyzw[i]);
				}
			}
			missingValue = std::sqrt(missingValue);
			*missingValueAddr = missingValue;
			return glm::quat(xyzw[3], xyzw[0], xyzw[1], xyzw[2]);
		}
		serialized_float combineFloat(serialized_float left, serialized_float right)
		{	
			return left << BIT_PER_FLOAT | right;
		}
		std::tuple<serialized_float, serialized_float> separateFloat(serialized_float value)
		{
			constexpr uint32_t MASK = createBitMask<16>();
			serialized_float right = value & MASK;
			serialized_float left = value >> 16;

			return { left, right };
		}
	}
}
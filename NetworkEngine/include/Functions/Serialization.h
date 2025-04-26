#pragma once
#include <Magnum/Math/Vector3.h>
#include <glm/gtc/quaternion.hpp>
#include "std.h"
namespace GDE
{
    namespace Serialization
    {
        typedef uint32_t serialized_float;
        typedef uint32_t serialized_quaternion;

        template<int T>
        constexpr uint32_t createBitMask()
        {
            uint32_t out = 0;
            for (int i = 0; i < T; i++)
            {
                out += (1 << i);
            }
            return out;
        }
        //DO NOT MODIFY
        constexpr int BIT_PER_FLOAT = 16;

        inline constexpr uint32_t FLOAT_BIT_MASK = createBitMask<BIT_PER_FLOAT>();
        inline constexpr uint32_t QUATERNION_BIT_MASK = createBitMask<10>();

        serialized_float serializeFloat(float value, int factor = 100);
        serialized_float serializeFloatQuaternion(float value, int factor = 1000);
        float deserializeFloat(serialized_float value, int factor = 100);
        float deserializeFloatQuaternion(serialized_float value, int factor = 1000);
        serialized_quaternion serializeQuaternion(const glm::quat& quat);
        glm::quat deserializeQuaternion(serialized_quaternion quat);
        serialized_float combineFloat(serialized_float left, serialized_float right);
        std::tuple<serialized_float, serialized_float> separateFloat(serialized_float value);

    }
}
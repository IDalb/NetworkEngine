#pragma once
// CRC-32 table and code generator
//
// Compile:
//  g++-10  -std=c++20 -DMY_CRC32_GENERATOR_TEST crc32-generator.cpp && ./a.out
//  clang++ -std=c++20 -DMY_CRC32_GENERATOR_TEST crc32-generator.cpp && ./a.out
//
// Usage:
//  #include <stdio.h>
//  #include <array>
//  #include "crc32-generator.cpp"
//  int main() {
//      const auto data = std::array<uint8_t, 5>{ 'H', 'e', 'l', 'l', 'o' };
//      Crc32::Generator crc32;
//      for(const uint8_t v : data) { crc32.update(v); }
//      printf("crc32.get()=0x%08x\n", crc32.get());        // crc32.get()=0xf7d18982
//  }
//
//  You can also give a reversed bit order polynomial as a template argument.
//
//  void test_crc32c() {
//      const auto data = std::array<uint8_t, 5>{ 'H', 'e', 'l', 'l', 'o' };
//      Crc32::Generator<0x82F63B78> crc32c;                // CRC-32C
//      for(const uint8_t v : data) { crc32c.update(v); }
//      printf("crc32c.get()=0x%08x\n", crc32c.get());      // crc32c.get()=0x81d90e1b
//  }
//
// License:
//   Copyright (C) 2021, Takayuki Matsuoka.
//   SPDX-License-Identifier: CC0-1.0
#include <stdint.h>
#include <array>

namespace Crc32 {
    // Note that bit order of polynomial representation is reversed.
    //
    // For example, 0xEDB88320 means reversed bit order representation of the following polynomial
    //   x^32 + x^26 + x^23 + x^22 + x^16 + x^12 + x^11 + x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x + 1
    //
    // Bit representation of above polynomial is
    //  bit#    32| 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0|
    //  coef     1|  0  0  0  0  0  1  0  0  1  1  0  0  0  0  0  1  0  0  0  1  1  1  0  1  1  0  1  1  0  1  1  1|
    //
    // High to low (normal) bit order value is 0x04C11DB7 (bit-32 is ignored)
    //  bit#    32| 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0|
    //  coef     1|  0  0  0  0: 0  1  0  0: 1  1  0  0: 0  0  0  1: 0  0  0  1: 1  1  0  1: 1  0  1  1: 0  1  1  1|
    //  order     |  8  4  2  1: 8  4  2  1: 8  4  2  1: 8  4  2  1: 8  4  2  1: 8  4  2  1: 8  4  2  1: 8  4  2  1|
    //  hex       |  0 = 0b0000: 4 = 0b0100: C = 0b1100: 1 = 0b0001: 1 = 0b0001: D = 0b1101: B = 0b1011: 7 = 0b0111|
    //
    // Low to high (reversed) bit order value is 0xEDB88320 (bit-32 is ignored)
    //  bit#    32| 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0|
    //  coef     1|  0  0  0  0: 0  1  0  0: 1  1  0  0: 0  0  0  1: 0  0  0  1: 1  1  0  1: 1  0  1  1: 0  1  1  1|
    //  order     |  1  2  4  8: 1  2  4  8: 1  2  4  8: 1  2  4  8: 1  2  4  8: 1  2  4  8: 1  2  4  8: 1  2  4  8|
    //  hex       |  0 = 0b0000: 2 = 0b0010: 3 = 0b0011: 8 = 0b1000: 8 = 0b1000: B = 0b1011: D = 0b1101: E = 0b1110|

    inline const uint32_t crc32_polynomial = 0xEDB88320;    // reverse bit order CRC-32 polynomial

    // Compile-time generic 32-bit CRC table generator
    // https://stackoverflow.com/a/36522355
    inline constexpr std::array<uint32_t, 256> gen_crc32_table(uint32_t polynomial) {
        auto crc32_table = std::array<uint32_t, 256>{};
        for (uint32_t byte = 0; byte < crc32_table.size(); ++byte) {
            uint32_t crc = byte;
            for (int i = 0; i < 8; ++i) {
                const auto m = crc & 1;
                crc >>= 1;
                if (m != 0) {
                    crc ^= polynomial;
                }
            }
            crc32_table[byte] = crc;
        }
        return crc32_table;
    }

    // compile-time generic 32-bit CRC code generation for byte array
    inline constexpr uint32_t crc32_generic(uint32_t polynomial, const auto* p, size_t len) {
        static_assert(sizeof(*p) == 1);  // Should we use concept?
        auto table = gen_crc32_table(polynomial);
        uint32_t crc = 0xffffffff;
        for (size_t i = 0; i < len; ++i) {
            crc = table[(crc ^ static_cast<uint8_t>(p[i])) & 0xff] ^ (crc >> 8);
        }
        return ~crc;
    }

    // compile-time CRC-32 code generation for byte array
    inline constexpr uint32_t crc32(const auto* p, size_t len) {
        return crc32_generic(crc32_polynomial, p, len);
    }

    // compile-time strlen()
    inline constexpr size_t constexpr_strlen(const auto* p) {
        const auto* s = p;
        while (*p != 0) {
            ++p;
        }
        return static_cast<size_t>(p - s);
    }

    // compile-time generic 32-bit CRC code generation for null-terminated char string
    inline constexpr uint32_t crc32(uint32_t polynomial, const char* p) {
        return crc32_generic(polynomial, p, constexpr_strlen(p));
    }

    // compile-time CRC-32 code generation for null-terminated char string
    inline constexpr uint32_t crc32(const char* p) {
        return crc32(p, constexpr_strlen(p));
    }

    template<unsigned Polynomial = crc32_polynomial>
    class Generator {
    public:
        static constexpr uint32_t polynomial = Polynomial;

        static constexpr auto table = gen_crc32_table(polynomial);

        void clear() {
            crc = 0xffffffff;
        }

        uint32_t get() const {
            return ~crc;
        }

        void update(uint8_t v) {
            crc = table[(crc ^ v) & 0xff] ^ (crc >> 8);
        }

    private:
        uint32_t crc = 0xffffffff;
    };
}

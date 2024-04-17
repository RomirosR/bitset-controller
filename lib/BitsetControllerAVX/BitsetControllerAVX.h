#pragma once

#include "immintrin.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>

template <size_t SIZE>
class BitsetControllerAVX{
   private:
    std::array<__m256i, ((SIZE + 255) >> 8)> a_;
    std::array<int64_t, 4> find_temp;

   public:
    BitsetControllerAVX() {
        for (int i = 0; i < ((SIZE + 255) >> 8); i++) {
            a_[i] = _mm256_setzero_si256();
        }
    }

    BitsetControllerAVX(const BitsetControllerAVX& other) = default;

    [[noexcept]] BitsetControllerAVX(BitsetControllerAVX&& other) = default;

    BitsetControllerAVX& operator=(const BitsetControllerAVX& other) = default;

    [[noexcept]] BitsetControllerAVX& operator=(BitsetControllerAVX&& other) = default;

    static __int64_t Get(__m256i a, std::size_t i) {
        return reinterpret_cast<uint64_t*>(&a)[i];
    }

    static __m256i Set(__m256i a, std::size_t i, __int64_t x) {
        reinterpret_cast<uint64_t*>(&a)[i] = x;
        return a;
    }

    inline void Add(std::size_t i) {
        const int c = i >> 8; // 0 <= c < ((SIZE + 255) >> 8)
        i &= 255; // 0 <= i <= 255
        auto temp = reinterpret_cast<uint64_t*>(&a_[c]);
        temp[i >> 6] |= (1ull << (i & 63));
    }

    bool Test(std::size_t i) {
        const int c = i >> 8; // 0 <= c < ((SIZE + 255) >> 8)
        i &= 255; // 0 <= i <= 255
        auto temp = reinterpret_cast<uint64_t*>(&a_[c]);
        return (temp[i >> 6] & (1ull << (i & 63))) != 0;
    }

    BitsetControllerAVX& operator|=(const BitsetControllerAVX& other) {
        for (int i = 0; i < ((SIZE + 255) >> 8); i++) {
            a_[i] = _mm256_or_si256(a_[i], other.a_[i]);
        }
        return *this;
    }

    BitsetControllerAVX operator|(const BitsetControllerAVX& other) {
        BitsetControllerAVX temp = *this;
        temp |= other;
        return temp;
    }

    BitsetControllerAVX& operator&=(const BitsetControllerAVX& other) {
        for (int i = 0; i < ((SIZE + 255) >> 8); i++) {
            a_[i] = _mm256_and_si256(a_[i], other.a_[i]);
        }
        return *this;
    }

    BitsetControllerAVX operator&(const BitsetControllerAVX& other) {
        BitsetControllerAVX temp = *this;
        temp &= other;
        return temp;
    }

    std::size_t Find_next(std::size_t i) {
        for (int j = 0; j < 4; j++) {
            find_temp[j] = Get(a_[i >> 8], j);
        }
        find_temp[(i & 255) >> 6] = find_temp[(i & 255) >> 6] & (~((1ull << (i & 63)) - 1));
        int j = (i & 255) >> 6;
        while (j < 4 && find_temp[j] == 0) {
            j++;
        }
        if (j < 4) {
            return ((i >> 8) << 8) + (j << 6) + __builtin_ctzll(find_temp[j]);
        }
        i += 256 - (i & 255);
        while (i < SIZE && _mm256_testz_si256(a_[i >> 8], a_[i >> 8])) {
            i += 256;
        }
        if (i >= SIZE) {
            return SIZE;
        }
        j = 0;
        while (j < 4 && Get(a_[i >> 8], j) == 0) {
            j++;
        }
        return ((i >> 8) << 8) + (j << 6) + __builtin_ctzll(Get(a_[i >> 8], j));
    } 
};
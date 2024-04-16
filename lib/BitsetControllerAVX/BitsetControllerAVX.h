#pragma once

#include "immintrin.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>

class BitsetControllerAVX{
   private:
    std::array<__m256i, 2> a_;
    std::array<int64_t, 4> find_temp;

   public:
    BitsetControllerAVX() : a_({_mm256_setzero_si256(), _mm256_setzero_si256()}) {
    }

    BitsetControllerAVX(const BitsetControllerAVX& other) = default;

    [[noexcept]] BitsetControllerAVX(BitsetControllerAVX&& other) = default;

    BitsetControllerAVX& operator=(const BitsetControllerAVX& other) = default;

    [[noexcept]] BitsetControllerAVX& operator=(BitsetControllerAVX&& other) = default;

    static __int64_t Get(__m256i a, std::size_t i) {
        if (i == 0) {
            return _mm256_extract_epi64(a, 0);
        }
        if (i == 1) {
            return _mm256_extract_epi64(a, 1);
        }
        if (i == 2) {
            return _mm256_extract_epi64(a, 2);
        }
        if (i == 3) {
            return _mm256_extract_epi64(a, 3);
        }
        return -1;
    }

    static __m256i Set(__m256i a, std::size_t i, __int64_t x) {
        if (i == 0) {
            return _mm256_insert_epi64(a, x, 0);
        }
        if (i == 1) {
            return _mm256_insert_epi64(a, x, 1);
        }
        if (i == 2) {
            return _mm256_insert_epi64(a, x, 2);
        }
        if (i == 3) {
            return _mm256_insert_epi64(a, x, 3);
        }
        return _mm256_setzero_si256();
    }

    void Add(std::size_t i) {
        const int c = i >> 8; // 0 <= c <= 1
        i &= 255; // 0 <= i <= 255
        __int64_t x = Get(a_[c], i >> 6);
        x |= (1ull << (i & 63));
        a_[c] = Set(a_[c], i >> 6, x);
    }

    bool Test(std::size_t i) {
        const int c = i >> 8; // 0 <= c <= 1
        i &= 255; // 0 <= i <= 255
        __int64_t x = Get(a_[c], i >> 6);
        return  (x & (1ull << (i & 63))) != 0;
    }

    BitsetControllerAVX& operator|=(const BitsetControllerAVX& other) {
        for (int i = 0; i < 2; i++) {
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
        for (int i = 0; i < 2; i++) {
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
        int j = 0;
        while (j < 4) {
            if (((i >> 8) << 8) + ((j + 1) << 6) <= i) {
                find_temp[j] = 0;
            }
            if (((i >> 8) << 8) <= i && i < ((i >> 8) << 8) + ((j + 1) << 6)) {
                find_temp[j] = find_temp[j] & (~((1ull << (i & 63)) - 1));
                break;
            }
            j++;
        }
        __int64_t or_val = 0;
        for (int k = j; k < 4; k++) {
            or_val |= find_temp[k];
        }
        if (or_val != 0) {
            while (j < 4 && find_temp[j] == 0) {
                j++;
            }
            return ((i >> 8) << 8) + (j << 6) + __builtin_ctzll(find_temp[j]);
        }
        i += 256 - (i & 255);
        while (i < 512 && _mm256_testz_si256(a_[i >> 8], a_[i >> 8])) {
            i += 256;
        }
        if (i >= 512) {
            return 512;
        }
        j = 0;
        while (j < 4 && Get(a_[i >> 8], j) == 0) {
            j++;
        }
        return ((i >> 8) << 8) + (j << 6) + __builtin_ctzll(Get(a_[i >> 8], j));
    } 
};
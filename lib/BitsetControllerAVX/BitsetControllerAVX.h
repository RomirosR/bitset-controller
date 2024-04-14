#pragma once

#include "immintrin.h"
#include <algorithm>

class BitsetControllerAVX{
   private:
    __m512i a_;

   public:
    BitsetControllerAVX() : a_(_mm512_setzero()){
    }

    BitsetControllerAVX(const BitsetControllerAVX& other) = default;

    [[noexcept]] BitsetControllerAVX(BitsetControllerAVX&& other) = default;

    BitsetControllerAVX& operator=(const BitsetControllerAVX& other) = default;

    [[noexcept]] BitsetControllerAVX(BitsetControllerAVX&& other) = default;
};
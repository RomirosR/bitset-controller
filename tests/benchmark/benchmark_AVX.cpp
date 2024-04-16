#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <iostream>
#include <random>
#include <vector>

#include "lib/BitsetControllerAVX/BitsetControllerAVX.h"

std::mt19937 rng(1337);

int RandFromRange(int l, int r) { return rng() % (r - l + 1) + l; }

template <size_t SZ> 
BitsetControllerAVX<SZ> RandomBitset() {
    BitsetControllerAVX<SZ> bc;
    const int k = RandFromRange(0, SZ);
    for (int i = 0; i < k; i++) {
        const int idx = RandFromRange(0, SZ - 1);
        bc.Add(idx);
    }
    return bc;
}

// template <size_t SZ> 
// BitsetControllerAVX<SZ> RandomBitsetAVX() {
//     BitsetControllerAVX<SZ> bc;
//     const int k = RandFromRange(0, SZ);
//     for (int i = 0; i < k; i++) {
//         const int idx = RandFromRange(0, SZ - 1);
//         bc.Add(idx);
//     }
//     return bc;
// }

TEST_CASE("Add and Test") {
    const size_t SIZE = 123;
    BitsetControllerAVX<SIZE> bc;
    BENCHMARK("[avx] Add") {
        const int idx = RandFromRange(0, SIZE - 1);
        bc.Add(idx);
        return;
    };

    BENCHMARK("[avx] Test") {
        const int idx = RandFromRange(0, SIZE - 1);
        bc.Test(idx);
        return;
    };
}
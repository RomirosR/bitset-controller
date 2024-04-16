#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <iostream>
#include <random>
#include <vector>

// #include "lib/BitsetControllerAVX/BitsetControllerAVX.h"
#include "lib/BitsetController/BitsetController.h"

std::mt19937 rng(1337);

int RandFromRange(int l, int r) { return rng() % (r - l + 1) + l; }

template <size_t SZ> 
BitsetController<SZ> RandomBitset() {
    BitsetController<SZ> bc;
    const int k = RandFromRange(0, SZ);
    for (int i = 0; i < k; i++) {
        const int idx = RandFromRange(0, SZ - 1);
        bc.Add(idx);
    }
    return bc;
}

template <size_t B_SZ, size_t O_SZ>
std::tuple<std::array<BitsetController<B_SZ>, O_SZ>, std::array<int, O_SZ>, std::array<int, O_SZ>> GenData() {
    std::array<BitsetController<B_SZ>, O_SZ> bc;
    for (auto& x : bc) {
        x = RandomBitset<B_SZ>();
    }
    std::array<int, O_SZ> order1;
    std::iota(order1.begin(), order1.end(), 0);
    std::shuffle(order1.begin(), order1.end(), rng);
    std::array<int, O_SZ> order2;
    std::iota(order2.begin(), order2.end(), 0);
    std::shuffle(order2.begin(), order2.end(), rng);
    return {bc, order1, order2};
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
    BitsetController<SIZE> bc;
    BENCHMARK("Add") {
        const int idx = RandFromRange(0, SIZE - 1);
        bc.Add(idx);
        return;
    };

    BENCHMARK("Test") {
        const int idx = RandFromRange(0, SIZE - 1);
        bc.Test(idx);
        return;
    };
}

TEST_CASE("Bitwise AND and OR") {
    // BENCHMARK_ADVANCED("Bitwise AND, size = 512")(Catch::Benchmark::Chronometer meter) {
    //     auto [bc, order1, order2] = GenData<512, 1024>();
    //     meter.measure([&](int i) {
    //         return bc[order1[i & 1023]] &= bc[order2[i & 1023]];
    //     });
    // };
    // BENCHMARK_ADVANCED("Bitwise AND, size = 8192")(Catch::Benchmark::Chronometer meter) {
    //     auto [bc, order1, order2] = GenData<8192, 1024>();
    //     meter.measure([&](int i) {
    //         return bc[order1[i & 1023]] &= bc[order2[i & 1023]];
    //     });
    // };
    // BENCHMARK_ADVANCED("Bitwise AND, size = 65536")(Catch::Benchmark::Chronometer meter) {
    //     auto [bc, order1, order2] = GenData<65536, 256>();
    //     meter.measure([&](int i) {
    //         return bc[order1[i & 255]] &= bc[order2[i & 255]];
    //     });
    // };
    // auto [bc, order1, order2] = GenData<16777216, 4>();
    // int i = 0;
    // REQUIRE(true);
    // while (true) {
    //     try {
    //             std::cerr << i << "\n";
    //             bc[order1[i & 3]] &= bc[order2[i & 3]];
    //     } catch (...) {

    //     }
    //     i++;

    // }
    BENCHMARK_ADVANCED("Bitwise AND, size = 16777216")(Catch::Benchmark::Chronometer meter) {
        auto [bc, order1, order2] = GenData<16777216, 4>();
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                bc[i] &= bc[j];
            }
        }
        // meter.measure([&](int i) {
        //     return bc[order1[i & 3]] &= bc[order2[i & 3]];
        // });
    };
}
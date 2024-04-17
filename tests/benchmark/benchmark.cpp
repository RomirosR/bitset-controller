#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <iostream>
#include <random>
#include <vector>
#include <fstream>

#include "lib/BitsetController/BitsetController.h"
#include <unistd.h>

std::mt19937 rng(1337);

int RandFromRange(int l, int r) { return rng() % (r - l + 1) + l; }

template <size_t SZ> 
BitsetController<SZ> RandomBitset(double lb = 0, double ub = 1) {
    BitsetController<SZ> bc;
    std::array<int, SZ> order;
    std::iota(order.begin(), order.end(), 0);
    std::shuffle(order.begin(), order.end(), rng);
    const int k = RandFromRange(SZ * lb, SZ * ub);
    for (int i = 0; i < k; i++) {
        const int idx = order[i];
        bc.Add(idx);
    }
    return bc;
}

template <size_t B_SZ, size_t O_SZ>
std::tuple<std::array<BitsetController<B_SZ>, O_SZ>, std::array<int, O_SZ>, std::array<int, O_SZ>> GenData(double lb = 0, double ub = 1) {
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
    BENCHMARK_ADVANCED("Bitwise AND, size = 512")(Catch::Benchmark::Chronometer meter) {
        auto [bc, order1, order2] = GenData<512, 1024>();
        meter.measure([&](int i) {
            return bc[order1[i & 1023]] &= bc[order2[i & 1023]];
        });
    };
    BENCHMARK_ADVANCED("Bitwise AND, size = 8192")(Catch::Benchmark::Chronometer meter) {
        auto [bc, order1, order2] = GenData<8192, 1024>();
        meter.measure([&](int i) {
            return bc[order1[i & 1023]] &= bc[order2[i & 1023]];
        });
    };
    BENCHMARK_ADVANCED("Bitwise AND, size = 65536")(Catch::Benchmark::Chronometer meter) {
        auto [bc, order1, order2] = GenData<65536, 256>();
        meter.measure([&](int i) {
            return bc[order1[i & 255]] &= bc[order2[i & 255]];
        });
    };

    BENCHMARK_ADVANCED("Bitwise OR, size = 512")(Catch::Benchmark::Chronometer meter) {
        auto [bc, order1, order2] = GenData<512, 1024>();
        meter.measure([&](int i) {
            return bc[order1[i & 1023]] |= bc[order2[i & 1023]];
        });
    };
    BENCHMARK_ADVANCED("Bitwise OR, size = 8192")(Catch::Benchmark::Chronometer meter) {
        auto [bc, order1, order2] = GenData<8192, 1024>();
        meter.measure([&](int i) {
            return bc[order1[i & 1023]] |= bc[order2[i & 1023]];
        });
    };
    BENCHMARK_ADVANCED("Bitwise OR, size = 65536")(Catch::Benchmark::Chronometer meter) {
        auto [bc, order1, order2] = GenData<65536, 256>();
        meter.measure([&](int i) {
            return bc[order1[i & 255]] |= bc[order2[i & 255]];
        });
    };
}

TEST_CASE("Find_next") {
    BENCHMARK_ADVANCED("Random data, single run, size = 65536")(Catch::Benchmark::Chronometer meter) {
        auto [bc, order1, order2] = GenData<65536, 256>();
        meter.measure([&](int i) {
            return bc[order1[i & 255]].Find_next(RandFromRange(0, 65536 - 1));
        });
    };
    BENCHMARK_ADVANCED("Random data, iterate over ones, size = 65536, popcnt ~ 70-100%")(Catch::Benchmark::Chronometer meter) {
        auto [bc, order1, order2] = GenData<65536, 256>(0.7, 1);
        meter.measure([&](int i) {
            volatile int j = bc[order1[i & 255]].Find_next(0);
            while (j < 65536) {
                j = bc[order1[i & 255]].Find_next(j + 1);
            }
            return j;
        });
    };
}
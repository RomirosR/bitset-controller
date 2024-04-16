#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <random>
#include <vector>

#include "lib/BitsetControllerAVX/BitsetControllerAVX.h"

std::mt19937 rng(1337);

const std::size_t SIZE = (1 << 9) * 10;
using Bitset = BitsetControllerAVX<SIZE>;

int RandFromRange(int l, int r) { return rng() % (r - l + 1) + l; }

std::pair<Bitset, std::vector<bool>> RandomBitsetAndActual() {
    Bitset bc;
    const int k = RandFromRange(0, SIZE);
    std::vector<bool> actual(SIZE);
    for (int i = 0; i < k; i++) {
        const int idx = RandFromRange(0, SIZE - 1);
        bc.Add(idx);
        actual[idx] = true;
    }
    return {bc, actual};
}

TEST_CASE("Size is multiple of 512") {
    REQUIRE(SIZE % 512 == 0);
}

TEST_CASE("Add and Test methods") {
    Bitset bc;
    for (int i = 0; i < 100; i++) {
        bc = Bitset();
        bc.Add(i);
        REQUIRE(bc.Test(i));
        for (int j = 0; j < SIZE; j++) {
            if (i == j) {
                continue;
            }
            REQUIRE(!bc.Test(j));
        }
    }
    for (int i = 0; i < 100; i++) {
        bc = Bitset();
        std::vector<bool> actual(SIZE);
        const int k = RandFromRange(0, SIZE);
        for (int j = 0; j < k; j++) {
            const int idx = RandFromRange(0, SIZE - 1);
            actual[idx] = true;
            bc.Add(idx);
        }
        for (int j = 0; j < SIZE; j++) {
            REQUIRE(actual[j] == bc.Test(j));
        }
    }
}

TEST_CASE("Bitwise OR") {
    for (int i = 0; i < 100; i++) {
        auto [bc1, actual1] = RandomBitsetAndActual();
        auto [bc2, actual2] = RandomBitsetAndActual();
        auto bc3 = bc1 | bc2;
        for (int j = 0; j < SIZE; j++) {
            REQUIRE((bc3.Test(j)) == (actual1[j] | actual2[j]));
        }
    }
    for (int i = 0; i < 100; i++) {
        auto [bc1, actual1] = RandomBitsetAndActual();
        auto [bc2, actual2] = RandomBitsetAndActual();
        bc1 |= bc2;
        for (int j = 0; j < SIZE; j++) {
            REQUIRE((bc1.Test(j)) == (actual1[j] | actual2[j]));
        }
    }
}

TEST_CASE("Bitwise AND") {
    for (int i = 0; i < 100; i++) {
        auto [bc1, actual1] = RandomBitsetAndActual();
        auto [bc2, actual2] = RandomBitsetAndActual();
        auto bc3 = bc1 & bc2;
        for (int j = 0; j < SIZE; j++) {
            REQUIRE((bc3.Test(j)) == (actual1[j] & actual2[j]));
        }
    }
    for (int i = 0; i < 100; i++) {
        auto [bc1, actual1] = RandomBitsetAndActual();
        auto [bc2, actual2] = RandomBitsetAndActual();
        bc1 &= bc2;
        for (int j = 0; j < SIZE; j++) {
            REQUIRE((bc1.Test(j)) == (actual1[j] & actual2[j]));
        }
    }
}

TEST_CASE("Find_next method") {
    for (int it = 0; it < 100; it++) {
        auto [bc, actual] = RandomBitsetAndActual();
        int cnt = 0;
        std::vector<bool> cur(SIZE);
        for (int i = bc.Find_next(0); i < SIZE; i = bc.Find_next(i + 1)) {
            cnt++;
            cur[i] = true;
        }
        REQUIRE(actual == cur);
        REQUIRE(cnt == std::count(actual.begin(), actual.end(), true));
    }
    for (int it1 = 0; it1 < 100; it1++) {
        auto [bc, actual] = RandomBitsetAndActual();
        for (int it2 = 0; it2 < 50; it2++) {
            const auto idx = static_cast<std::size_t>(RandFromRange(0, SIZE - 1));
            auto actual_idx = idx;
            while (actual_idx < SIZE && !actual[actual_idx]) {
                actual_idx++;
            }
            REQUIRE(actual_idx == bc.Find_next(idx));
        }
    }
}
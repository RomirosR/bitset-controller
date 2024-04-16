#pragma once 

#include <array>
#include <cstdint>
#include <iostream>

template <size_t SIZE>
class BitsetController {
   private:
    std::array<uint64_t, (SIZE >> 6)> a_;

   public:
    BitsetController() : a_() {}

    BitsetController(const BitsetController& other) = default;

    [[noexcept]] BitsetController(BitsetController&& other) = default;

    BitsetController& operator=(const BitsetController& other) = default;

    [[noexcept]] BitsetController& operator=(BitsetController&& other) = default;

    ~BitsetController() = default;

    bool operator==(const BitsetController& other) {
        return a_ == other.a_;
    }

    void Add(std::size_t i) {
        a_[i >> 6] |= (1ull << (i & 63));
    }

    bool Test(std::size_t i) {
        return (a_[i >> 6] & (1ull << (i & 63))) != 0ull;
    }

    BitsetController& operator|=(const BitsetController& other) {
        for (int i = 0; i < (SIZE >> 6); i++) {
            a_[i] |= other.a_[i];
        }
        return *this;
    }

    BitsetController operator|(const BitsetController& other) {
        BitsetController temp = *this;
        temp |= other;
        return temp;
    }

    BitsetController& operator&=(const BitsetController& other) {
        for (int i = 0; i < (SIZE >> 6); i++) {
            a_[i] &= other.a_[i];
        }
        return *this;
    }

    BitsetController operator&(const BitsetController& other) {
        BitsetController temp = *this;
        temp &= other;
        return temp;
    }

    std::size_t Find_next(std::size_t i) {
        if (i >= SIZE) {
            return SIZE;
        }
        if ((a_[i >> 6] & (~((1ull << (i & 63)) - 1))) != 0) {
            return ((i >> 6) << 6) + __builtin_ctzll(a_[i >> 6] & (~((1ull << (i & 63)) - 1)));
        }
        i += 64 - (i & 63);
        while (i < SIZE && a_[i >> 6] == 0) {
            i += 64;
        }
        if (i >= SIZE) {
            return SIZE;
        }
        return i + __builtin_ctzll(a_[i >> 6]);
    }
};
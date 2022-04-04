#ifndef _Random_
#define _Random_

#include <stdint.h>
#include <algorithm>
#include <random>

class Random {
private:
    enum : uint32_t {
        M = 2147483647L  // 2^31-1
    };
    enum : uint64_t {
        A = 16807  // bits 14, 8, 7, 5, 2, 1, 0
    };

    uint32_t seed_;

    static uint32_t GoodSeed(uint32_t s) { return (s & M) != 0 ? (s & M) : 1; }

public:
    // This is the largest value that can be returned from Next()
    enum : uint32_t {
        kMaxNext = M
    };

    explicit Random(uint32_t s) : seed_(GoodSeed(s)) {}

    void Reset(uint32_t s) { seed_ = GoodSeed(s); }

    uint32_t Next() {
        // We are computing
        //       seed_ = (seed_ * A) % M,    where M = 2^31-1
        //
        // seed_ must not be zero or M, or else all subsequent computed values
        // will be zero or M respectively.  For all other values, seed_ will end
        // up cycling through every number in [1,M-1]
        uint64_t product = seed_ * A;

        // Compute (product % M) using the fact that ((x << 31) % M) == x.
        seed_ = static_cast<uint32_t>((product >> 31) + (product & M));
        // The first reduction may overflow by 1 bit, so we may need to
        // repeat.  mod == M is not possible; using > allows the faster
        // sign-bit-based test.
        if (seed_ > M) {
            seed_ -= M;
        }
        return seed_;
    }

    uint64_t Next64() { return (uint64_t{Next()} << 32) | Next(); }

    // Returns a uniformly distributed value in the range [0..n-1]
    // REQUIRES: n > 0
    uint32_t Uniform(int n) { return Next() % n; }

    // Randomly returns true ~"1/n" of the time, and false otherwise.
    // REQUIRES: n > 0
    bool OneIn(int n) { return Uniform(n) == 0; }

    // "Optional" one-in-n, where 0 or negative always returns false
    // (may or may not consume a random value)
    bool OneInOpt(int n) { return n > 0 && OneIn(n); }

    // Returns random bool that is true for the given percentage of
    // calls on average. Zero or less is always false and 100 or more
    // is always true (may or may not consume a random value)
    bool PercentTrue(int percentage) {
        return static_cast<int>(Uniform(100)) < percentage;
    }

    // Skewed: pick "base" uniformly from range [0,max_log] and then
    // return "base" random bits.  The effect is to pick a number in the
    // range [0,2^max_log-1] with exponential bias towards smaller numbers.
    uint32_t Skewed(int max_log) {
        return Uniform(1 << Uniform(max_log + 1));
    }

    // Generates a random string of len bytes using human-readable characters
    std::string HumanReadableString(uint64_t len);
};

#endif
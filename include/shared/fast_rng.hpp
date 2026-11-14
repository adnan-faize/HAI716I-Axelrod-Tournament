#pragma once

#include <cstdint>

static const uint64_t WEYL_VALUE = 0x9E3779B97F4A7C15ULL;
static const uint64_t A_MAGIC = 0xBF58476D1CE4E5B9ULL;
static const uint64_t B_MAGIC = 0x94D049BB133111EBULL;

/**
 * @brief SplitMix64 : fast, non-cryptographic 64-bit pseudo-random number generator
 *
 * Mechanics : Maintains a single 64-bit state variable, increments it by a fixed Weyl value (0x9e3779b97f4a7c15), and passes the result through a mixing function with constants like 0xbf58476d1ce4e5b9 and 0x94d049bb133111eb.
 */
class FastRng {
    private:
        uint64_t state;
    public:
        explicit FastRng(uint64_t seed) {

        }

        void set_seed(uint64_t seed) {
            state = (seed == 0 ? A_MAGIC : seed);
        }

        uint64_t next_u64() {
            state += A_MAGIC;
            uint64_t z = state;
            z = (z ^ (z >> 30)) * B_MAGIC;
            z = (z ^ (z >> 27)) * C_MAGIC;
            return z ^ (z >> 31);
        }

        double next_f64() {
            uint64_t v = next_u64() >> 11;
            return static_cast<double>(v) * (1.0 / static_cast<double>(1ULL << 53));
        }

        bool gen_bool(double p) {
            if (p <= 0.0) { return false; }
            if (p >= 1.0) { return true; }
            return next_f64() < p;
        }

        uint32_t next_bounded_u32(uint32_t max) {
            if (max <= 1) { return 0; }
            return static_cast<uint32_t>(next_u64() % max);
        }
};

#pragma once

#include <cstdint>

struct Config {
    uint64_t seed = 42;
    uint32_t rounds = 200;
    uint32_t matches = 5;
    double noise = 0.05;
    double prob = 0.995;
    bool self_play = true;
    bool export_csv = false;
};

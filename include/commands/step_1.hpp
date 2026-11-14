#pragma once

#include <cstdlib>
#include <iostream>

#include "../shared/config.hpp"
#include "../shared/fast_rng.hpp"

inline int run_step_1(const Config& cfg) {
    std::cout << "" << std::endl;

    FastRng rng(cfg.seed);

    return EXIT_SUCCESS;
}

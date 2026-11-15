#pragma once

#include <cstdlib>
#include <iostream>

#include "../strategies/strategies.hpp"
#include "../shared/config.hpp"
#include "../utils/fast_rng.hpp"

inline int run_step_1(const Config& cfg) {
    std::cout << "" << std::endl;

    FastRng rng(cfg.seed);
    auto strategies = get_base_strategies();

    return EXIT_SUCCESS;
}

#pragma once

#include <memory>
#include <vector>

#include "../strategies/strategy.hpp"
#include "../strategies/all_d_strategy.hpp"
#include "../strategies/random_strategy.hpp"
#include "../strategies/tit_for_tat_strategy.hpp"
#include "../strategies/tester_strategy.hpp"
#include "../strategies/joss_strategy.hpp"

#include "../strategies/friedman_strategy.hpp"
#include "../strategies/pavlov_strategy.hpp"
#include "../strategies/tit_for_two_tats_strategy.hpp"
#include "../strategies/suspicious_tit_for_tat_strategy.hpp"
#include "../strategies/generous_tit_for_tat_strategy.hpp"

inline std::vector<std::unique_ptr<Strategy>> get_base_strategies() {
    std::vector<std::unique_ptr<Strategy>> vec;
    vec.push_back(std::make_unique<AllDStrategy>());
    vec.push_back(std::make_unique<RandomStrategy>());
    vec.push_back(std::make_unique<TitForTatStrategy>());
    vec.push_back(std::make_unique<TesterStrategy>());
    vec.push_back(std::make_unique<JossStrategy>());
    return vec;
}

inline std::vector<std::unique_ptr<Strategy>> get_extended_strategies() {
    std::vector<std::unique_ptr<Strategy>> vec = get_base_strategies();
    // vec.push_back(std::make_unique<AllDStrategy>());k
    return vec;
}

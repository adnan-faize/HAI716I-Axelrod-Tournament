#pragma once

#include "game.hpp"
#include <cstdint>
#include <optional>

struct TournamentCfg {
    uint32_t rounds = 200;
    uint32_t matches_per_pair = 5;
    double noise = 0.0;
    std::optional<double> continuation_prob = std::nullopt;
    PayoffMatrix payoff_matrix = PayoffMatrix::standard_pd();
    bool include_self_play = true;
};

struct MatchResult {

};

struct StrategyStats {

};

struct TournamentResult {

};

class TournamentEngine {

};

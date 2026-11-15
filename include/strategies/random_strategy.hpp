#pragma once

#include <memory>
#include <string>

#include "./strategy.hpp"

class RandomStrategy : public Strategy {
    public:
        std::string name() const override;
        std::string description() const override;
        std::string expected_behavior() const override;

        Move choose(const std::vector<Move>&, const std::vector<Move>&, FastRng&) override;

        std::unique_ptr<Strategy> clone() const override;
};

inline std::string RandomStrategy::name() const {
    return "RANDOM";
}

inline std::string RandomStrategy::description() const {
    return "Joue Coopérer (C) ou Trahir (D) de façon aléatoire (50/50).";
}

inline std::string RandomStrategy::expected_behavior() const {
    return "Généralement mauvais classement. ALL-D gagne à chaque C et détruit la coopération avec TFT et Friedman.";
}

inline Move RandomStrategy::choose(const std::vector<Move>&, const std::vector<Move>&, FastRng& rng) {
    return rng.gen_bool(0.5) ? Move::C : Move::D;
}

inline std::unique_ptr<Strategy> RandomStrategy::clone() const {
    return std::make_unique<RandomStrategy>(*this);
}

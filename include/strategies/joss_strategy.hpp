#pragma once

#include <string>

#include "./strategy.hpp"

class JossStrategy : public Strategy {
    public:
        std::string name() const override;
        std::string description() const override;
        std::string author_origin() const override;
        std::string expected_behavior() const override;

        Move choose(const std::vector<Move>&, const std::vector<Move>&, FastRng&) override;

        std::unique_ptr<Strategy> clone() const override;
};

inline std::string JossStrategy::name() const {
    return "JOSS";
}

inline std::string JossStrategy::description() const {
    return "Joue TFT, mais trahit sournoisement avec 10% de probabilité au lieu de coopérer.";
}


inline std::string JossStrategy::author_origin() const {
    return "Johann Joss, soumise au premier tournoi d'Axelrod (1980).";
}


inline std::string JossStrategy::expected_behavior() const {
    return "Tente de grappiller des points face aux coopérateurs, mais déclenche des vendettas destructrices (cycles infinis d'alternance C/D ou D/D) face à TFT ou Friedman.";
}

inline Move JossStrategy::choose(const std::vector<Move>&, const std::vector<Move>& opponent, FastRng& rng) {
    if (opponent.empty()) { return Move::C; }
    Move last_opp = opponent.back();
    if (last_opp == Move::D) { return Move::D; }
    return rng.gen_bool(0.10) ? Move::D : Move::C;
}

inline std::unique_ptr<Strategy> JossStrategy::clone() const {
    return std::make_unique<JossStrategy>(*this);
}

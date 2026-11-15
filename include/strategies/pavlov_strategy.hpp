#pragma once

#include <string>

#include "./strategy.hpp"

class PavlovStrategy : public Strategy {
    public:
        std::string name() const override;
        std::string description() const override;
        std::string author_origin() const override;
        std::string expected_behavior() const override;

        Move choose(const std::vector<Move>&, const std::vector<Move>&, FastRng&) override;

        std::unique_ptr<Strategy> clone() const override;
};

inline std::string PavlovStrategy::name() const {
    return "Pavlov";
}

inline std::string PavlovStrategy::description() const {
    return "Commence par C. Coopère si les deux joueurs ont fait le même choix au tour précédent ; trahit s'ils ont fait des choix différents.";
}


inline std::string PavlovStrategy::author_origin() const {
    return "Martin Nowak et Karl Sigmund (Nature, 1993), stratégie d'apprentissage conditionné.";
}


inline std::string PavlovStrategy::expected_behavior() const {
    return "Excellente en présence de bruit car elle sait corriger une double erreur, et exploite systématiquement les joueurs inconditionnellement coopératifs (ALL-C).";
}

inline Move PavlovStrategy::choose(const std::vector<Move>& me, const std::vector<Move>& opponent, FastRng&) {
    if (opponent.empty()) { return Move::C; }
    Move my_last = me.back();
    Move opp_last = opponent.back();
    return (my_last == opp_last) ? Move::C : Move::D;
}

inline std::unique_ptr<Strategy> PavlovStrategy::clone() const {
    return std::make_unique<PavlovStrategy>(*this);
}

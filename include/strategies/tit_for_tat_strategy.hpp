#pragma once

#include <memory>
#include <string>

#include "./strategy.hpp"

class TitForTatStrategy : public Strategy {
    public:
        std::string name() const override;
        std::string description() const override;
        std::string author_origin() const override;
        std::string expected_behavior() const override;

        Move choose(const std::vector<Move>&, const std::vector<Move>&, FastRng&) override;

        std::unique_ptr<Strategy> clone() const override;
};

inline std::string TitForTatStrategy::name() const {
    return "TIT-FOR-TAT";
}

inline std::string TitForTatStrategy::description() const {
    return "Commence par Coopérer puis imite le coup précédent de l'adversaire";
}

inline std::string TitForTatStrategy::author_origin() const {
    return "Proposée par Anatol Rapoport, gagnante des tournois 1 et 2 d'Axelrod.";
}

inline std::string TitForTatStrategy::expected_behavior() const {
    return "Excellente dans des populations coopératives grâce à sa gentillesse, sa capacité de représailles immédiates et son pardon rapide ; peut souffrir si l'environnement est dominé par des traîtres agressifs.";
}

inline Move TitForTatStrategy::choose(const std::vector<Move>&, const std::vector<Move>& opponent, FastRng&) {
    if (opponent.empty()) { return Move::C; }
    return opponent.back();
}

inline std::unique_ptr<Strategy> TitForTatStrategy::clone() const {
    return std::make_unique<TitForTatStrategy>(*this);
}

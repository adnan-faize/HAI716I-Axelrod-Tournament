#pragma once

#include <string>

#include "./strategy.hpp"

class FriedmanStrategy : public Strategy {
    private:
        bool triggered = false;

    public:
        std::string name() const override;
        std::string description() const override;
        std::string author_origin() const override;
        std::string expected_behavior() const override;

        void reset() override;

        Move choose(const std::vector<Move>&, const std::vector<Move>&, FastRng&) override;

        std::unique_ptr<Strategy> clone() const override;
};

inline std::string FriedmanStrategy::name() const {
    return "FRIEDMAN";
}

inline std::string FriedmanStrategy::description() const {
    return "Commence par C et coopère tant que l'adversaire n'a jamais trahi. À la moindre trahison, trahit pour toujours.";
}


inline std::string FriedmanStrategy::author_origin() const {
    return "James W. Friedman (1971), première stratégie de 'Grim Trigger' analysée en théorie des jeux répétés.";
}


inline std::string FriedmanStrategy::expected_behavior() const {
    return "Parfaite face aux stratégies gentilles (maintient (3,3)) ; intraitable avec ALL-D, JOSS ou RANDOM, mais ne pardonne jamais aucune maladresse (très vulnérable au bruit).";
}

inline void FriedmanStrategy::reset() {
    triggered = false;
}

inline Move FriedmanStrategy::choose(const std::vector<Move>&, const std::vector<Move>& opponent, FastRng& rng) {
    if (triggered) { return Move::D; }
    if (!opponent.empty() && is_defection(opponent.back())) {
        triggered = true;
        return Move::D;
    }
    return Move::C;
}

inline std::unique_ptr<Strategy> FriedmanStrategy::clone() const {
    return std::make_unique<FriedmanStrategy>(*this);
}

#pragma once

#include <string>

#include "./strategy.hpp"

class TesterStrategy : public Strategy {
    private:
        bool has_apologized = false;
        bool in_tft_mode = false;

    public:
        std::string name() const override;
        std::string description() const override;
        std::string author_origin() const override;
        std::string expected_behavior() const override;

        void reset() override;

        Move choose(const std::vector<Move>&, const std::vector<Move>&, FastRng&) override;

        std::unique_ptr<Strategy> clone() const override;
};

inline std::string TesterStrategy::name() const {
    return "TESTER";
}

inline std::string TesterStrategy::description() const {
    return "Trahit au coup 1 pour sonder l'adversaire. S'il y a représailles, s'excuse par C puis joue TFT. Sinon, exploite en alternant C et D.";
}

inline std::string TesterStrategy::author_origin() const {
    return "David Gladstein, soumise au second tournoi d'Axelrod.";
}

inline std::string TesterStrategy::expected_behavior() const {
    return "Réussit à exploiter les stratégies trop indulgentes ; face à TFT, le test initial entraîne une réplique, puis Tester s'excuse et la coopération mutuelle s'établit durablement.";
}

inline void TesterStrategy::reset() {
    has_apologized = false;
    in_tft_mode = false;
}

inline Move TesterStrategy::choose(const std::vector<Move>&, const std::vector<Move>& opponent, FastRng&) {
    size_t round = opponent.size() + 1;

    if (round == 1) { return Move::D; }
    if (in_tft_mode) { return opponent.back(); }

    bool opponent_has_defected = false;

    for (Move m : opponent) {
        if (is_defection(m)) {
            opponent_has_defected = true;
            break;
        }
    }

    if (opponent_has_defected) {
        if (!has_apologized) {
            has_apologized = true;
            return Move::C;
        }

        in_tft_mode = true;
        return opponent.back();
    }

    if (round <= 3) { return Move::C; }
    return (round % 2 == 0) ? Move::D : Move::C;
}

inline std::unique_ptr<Strategy> TesterStrategy::clone() const {
    return std::make_unique<TesterStrategy>(*this);
}

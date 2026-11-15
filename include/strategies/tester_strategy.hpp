#pragma once

#include <string>

#include "./strategy.hpp"

class TesterStrategy : public Strategy {
    public:
        std::string name() const override;
        std::string description() const override;
        std::string author_origin() const override;
        std::string expected_behavior() const override;

        Move choose(const std::vector<Move>&, const std::vector<Move>&, FastRng&) override;

        std::unique_ptr<Strategy> clone() const override;
};

inline std::string TesterStrategy::name() const {
    return "TESTER";
}

inline std::string TesterStrategy::description() const {
    return "";
}

inline std::string TesterStrategy::author_origin() const {
    return "";
}

inline std::string TesterStrategy::expected_behavior() const {
    return "";
}

inline Move TesterStrategy::choose(const std::vector<Move>&, const std::vector<Move>& opponent, FastRng&) {
    if (opponent.empty()) { return Move::C; }
    return opponent.back();
}

inline std::unique_ptr<Strategy> TesterStrategy::clone() const {
    return std::make_unique<TitForTatStrategy>(*this);
}

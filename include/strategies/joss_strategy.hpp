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
    return "";
}


inline std::string JossStrategy::author_origin() const {
    return "";
}


inline std::string JossStrategy::expected_behavior() const {
    return "";
}

inline Move JossStrategy::choose(const std::vector<Move>&, const std::vector<Move>& opponent, FastRng&) {

}

inline std::unique_ptr<Strategy> JossStrategy::clone() const {
    return std::make_unique<JossStrategy>(*this);
}

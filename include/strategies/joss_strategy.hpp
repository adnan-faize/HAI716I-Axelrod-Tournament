#pragma once

#include <string>

#include "./strategy.hpp"

class JossStrategy : public Strategy {
    public:
        std::string name() const override;
        std::string description() const override;
        std::string expected_behavior() const override;
};

inline std::string JossStrategy::name() const {
    return "JOSS";
}

inline std::string JossStrategy::description() const {
    return "";
}

inline std::string JossStrategy::expected_behavior() const {
    return "";
}

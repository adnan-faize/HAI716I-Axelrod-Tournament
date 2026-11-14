#pragma once

#include <string>
#include <vector>

#include "../game.hpp"

/**
 * @brief Abstract class to implement the different strategies
 */
class Strategy {
    public:
        virtual ~Strategy() = default;

        virtual std::string name() const = 0;
        virtual std::string description() const = 0;
        virtual std::string expected_behavior() const = 0;

        virtual void reset() {}

        virtual Move choose(const std::vector<Move>& history, const std::vector<Move>& opponent_history) = 0;

};

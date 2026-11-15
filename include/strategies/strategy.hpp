#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../game.hpp"
#include "../utils/fast_rng.hpp"

/**
 * @brief Abstract class to implement the different strategies
 */
class Strategy {
    public:
        virtual ~Strategy() = default;

        virtual std::string name() const = 0;
        virtual std::string description() const = 0;
        virtual std::string author_origin() const = 0;
        virtual std::string expected_behavior() const = 0;

        virtual void reset() {}

        virtual Move choose(const std::vector<Move>& history, const std::vector<Move>& opponent_history, FastRng& rng) = 0;

        virtual std::unique_ptr<Strategy> clone() const = 0;
};

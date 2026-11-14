#include <string>

#include "./strategy.hpp"

class RandomStrategy : public Strategy {
    public:
        std::string name() const override;
        std::string description() const override;
        std::string expected_behavior() const override;
};

inline std::string RandomStrategy::name() const {
    return "RANDOM";
}

inline std::string RandomStrategy::description() const {
    return "";
}

inline std::string RandomStrategy::expected_behavior() const {
    return "";
}

#include <string>

#include "./strategy.hpp"

class TesterStrategy : public Strategy {
    public:
        std::string name() const override;
        std::string description() const override;
        std::string expected_behavior() const override;
};

inline std::string TesterStrategy::name() const {
    return "TESTER";
}

inline std::string TesterStrategy::description() const {
    return "";
}

inline std::string TesterStrategy::expected_behavior() const {
    return "";
}

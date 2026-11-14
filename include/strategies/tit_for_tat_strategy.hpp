#include <string>

#include "./strategy.hpp"

class TitForTatStrategy : public Strategy {
    public:
        std::string name() const override;
        std::string description() const override;
        std::string expected_behavior() const override;
};

inline std::string TitForTatStrategy::name() const {
    return "TIT-FOR-TAT";
}

inline std::string TitForTatStrategy::description() const {
    return "";
}

inline std::string TitForTatStrategy::expected_behavior() const {
    return "";
}

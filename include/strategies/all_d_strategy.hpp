#include <string>

#include "./strategy.hpp"

class AllDStrategy : public Strategy {
    public:
        std::string name() const override;
        std::string description() const override;
        std::string expected_behavior() const override;
};

inline std::string AllDStrategy::name() const {
    return "ALL-D";
}

inline std::string AllDStrategy::description() const {
    return "Joue trahir (D) à chaque tour.";
}

inline std::string AllDStrategy::expected_behavior() const {
    return "Performant fâce aux stratégies naîves ou aléatoires. Médiocre fâce à lui-même ou fâce à TFT car la coopération mutuelle n'est jamais établie (score bloqué à P=2).";
}

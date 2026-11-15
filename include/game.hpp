#pragma once

#include <string_view>
#include <iostream>

enum class Move : char {
    C = 'C', // Coopérer
    D = 'D', // Trahir
};

inline Move invert(Move m) {
    return (m == Move::C) ? Move::D : Move::C;
}

inline bool is_cooperation(Move m) {
    return m == Move::C;
}

inline char to_char(Move m) {
    return static_cast<char>(m);
}

inline std::ostream& operator<<(std::ostream& os, Move m) {
    os << to_char(m);
    return os;
}

/**
 * @brief Gains Matrix 2x2
 *
 *        |     P2 : C     | P2 : D
 * -------|----------------|--------------
 * P1 : C | R (Récompense) | S (Dupe)
 * P1 : D | T (Tentation)  | P (Punition)
 */
struct PayoffMatrix {
    std::string_view name;
    double r; // reward     (C,C)
    double s; // sucker     (C,D)
    double t; // temptation (D,C)
    double p; // punishment (D,D)

    PayoffMatrix(std::string_view name, double r, double s, double t, double p) {
        this->name = name;
        this->r = r;
        this->s = s;
        this->t = t;
        this->p = p;
    }

    // Standard prisoner dilemma - Axelrod 1980 (T=5,R=3,P=2,S=0)
    static PayoffMatrix standard_pd() {
        return PayoffMatrix("Dilemme du Prisonnier classique (Axelrod) : T=5,R=3,P=2,S=0", 5, 3, 2, 0);
    }

    // Severe prisoner dilemma (T=3,R=0,P=6,S=1)
    static PayoffMatrix severe_pd() {
        return PayoffMatrix("Dilemme du Prisonnier sévère : T=3,R=0,P=6,S=1", 3, 0, 6, 1);
    }

    std::pair<double, double> payoffs(Move a, Move b) const;
};

inline std::pair<double, double> PayoffMatrix::payoffs(Move a, Move b) const {
    if (a == Move::C && b == Move::C) { return { r, r }; }
    if (a == Move::C && b == Move::D) { return { s, t }; }
    if (a == Move::D && b == Move::C) { return { t, s }; }
    return { p, p };
}

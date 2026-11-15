#pragma once

#include <cstdlib>
#include <iostream>

#include "../strategies/strategies.hpp"
#include "../shared/config.hpp"
#include "../utils/fast_rng.hpp"
#include "../tournament.hpp"
#include "../output/console_output.hpp"
#include "../output/csv_output.hpp"
#include "../output/chart_output.hpp"

inline int run_step_3_gains(const Config& cfg) {
    ConsoleOutput::print_section(
        "ÉTAPE 3 (MODIF 4) : AUTRES MATRICES DE GAINS (SECTION 07 DU COURS)\n"
        "  Test sur deux jeux 2x2 emblématiques : Poule Mouillée (Chicken) et Chasse au Cerf (Stag Hunt)"
    );

    FastRng rng(cfg.seed);
    auto strategies = get_extended_strategies();

    // 1. Jeu de la Poule Mouillée (Chicken / Hawk-Dove) : T > R > S > P
    std::cout << "\n--- 1. JEU DE LA POULE MOUILLÉE / CHICKEN (T=5, R=3, S=1, P=0) ---\n";

    TournamentCfg cfg_chicken;
    cfg_chicken.rounds = cfg.rounds;
    cfg_chicken.matches_per_pair = cfg.matches;
    cfg_chicken.payoff_matrix = PayoffMatrix::chicken();
    cfg_chicken.include_self_play = cfg.self_play;

    auto res_chicken = TournamentEngine::run_tournament(strategies, cfg_chicken, rng);
    ChartOutput::generate_rankings_svg(
        res_chicken,
        "Étape 3 : Classement Poule Mouillée (Chicken)",
        "assets/step3_chicken_rankings.svg"
    );
    ConsoleOutput::print_rankings(res_chicken, "CLASSEMENT : JEU DE LA POULE MOUILLÉE (CHICKEN)");

    // 2. Chasse au Cerf (Stag Hunt) : R > T > P > S
    std::cout << "\n--- 2. CHASSE AU CERF / STAG HUNT (R=4, T=3, P=2, S=0) ---\n";

    TournamentCfg cfg_stag;
    cfg_stag.rounds = cfg.rounds;
    cfg_stag.matches_per_pair = cfg.matches;
    cfg_stag.payoff_matrix = PayoffMatrix::stag_hunt();
    cfg_stag.include_self_play = cfg.self_play;

    auto res_stag = TournamentEngine::run_tournament(strategies, cfg_stag, rng);
    ChartOutput::generate_rankings_svg(
        res_stag,
        "Étape 3 : Classement Chasse au Cerf (Stag Hunt)",
        "assets/step3_stag_rankings.svg"
    );
    ConsoleOutput::print_rankings(res_stag, "CLASSEMENT : CHASSE AU CERF (STAG HUNT)");

    if (cfg.export_csv) {
        CsvOutput::export_rankings_csv(res_chicken, "data/step3_chicken_rankings.csv");
        CsvOutput::export_rankings_csv(res_stag, "data/step3_stag_rankings.csv");
        std::cout << "[CSV] Fichiers exportés : data/step3_chicken_rankings.csv et data/step3_stag_rankings.csv\n";
    }

    return EXIT_SUCCESS;
}

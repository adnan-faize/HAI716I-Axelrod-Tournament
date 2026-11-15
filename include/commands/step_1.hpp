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

inline int run_step_1(const Config& cfg) {
    ConsoleOutput::print_section(
        "ÉTAPE 1 : TOURNOI DE BASE (5 STRATÉGIES DU COURS)\n"
        "  Format : 5 parties de 200 tours par paire (Matrice standard : T=5, R=3, P=2, S=0)"
    );

    FastRng rng(cfg.seed);
    auto strategies = get_base_strategies();

    TournamentCfg t_cfg;
    t_cfg.rounds = cfg.rounds;
    t_cfg.matches_per_pair = cfg.matches;
    t_cfg.noise = 0.0;
    t_cfg.continuation_prob = std::nullopt;
    t_cfg.payoff_matrix = PayoffMatrix::standard_pd();
    t_cfg.include_self_play = cfg.self_play;

    TournamentResult result = TournamentEngine::run_tournament(strategies, t_cfg, rng);

    ConsoleOutput::print_rankings(result, "CLASSEMENT ÉTAPE 1 (Tournoi Standard Axelrod - 5 stratégies du cours)");
    ConsoleOutput::print_head_to_head(result);

    ConsoleOutput::print_match_trace(result, "TIT-FOR-TAT", "JOSS", 20);
    ConsoleOutput::print_match_trace(result, "TIT-FOR-TAT", "TESTER", 10);

    if (cfg.export_csv) {
        CsvOutput::export_rankings_csv(result, "data/step1_rankings.csv");
        CsvOutput::export_head_to_head_csv(result, "data/step1_head_to_head.csv");
        std::cout << "[CSV] Fichiers exportés : data/step1_rankings.csv et data/step1_head_to_head.csv\n";
    }

    if (cfg.export_svg) {
        ChartOutput::generate_rankings_svg(
            result,
            "Étape 1 : Classement du Tournoi de Base (5 Stratégies)",
            "assets/step1_rankings.svg"
        );
    }

    return EXIT_SUCCESS;
}

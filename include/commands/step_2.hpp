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

inline int run_step_2(const Config& cfg) {
    ConsoleOutput::print_section(
        "ÉTAPE 2 : NOUVELLES STRATÉGIES (POPULATION ÉLARGIE DE 7 STRATÉGIES)\n"
        "  Ajouts (2 stratégies) : FRIEDMAN (Grim Trigger) et PAVLOV (Win-Stay, Lose-Shift)"
    );

    FastRng rng(cfg.seed);
    auto strategies = get_extended_strategies();

    TournamentCfg t_cfg;
    t_cfg.rounds = cfg.rounds;
    t_cfg.matches_per_pair = cfg.matches;
    t_cfg.noise = 0.0;
    t_cfg.continuation_prob = std::nullopt;
    t_cfg.payoff_matrix = PayoffMatrix::standard_pd();
    t_cfg.include_self_play = cfg.self_play;

    TournamentResult result = TournamentEngine::run_tournament(strategies, t_cfg, rng);

    ChartOutput::generate_rankings_svg(
        result,
        "Étape 2 : Classement du Tournoi Élargi (7 Stratégies)",
        "assets/step2_rankings.svg"
    );

    ConsoleOutput::print_rankings(result, "CLASSEMENT ÉTAPE 2 (Tournoi Élargi - 7 stratégies)");
    ConsoleOutput::print_head_to_head(result);

    if (cfg.export_csv) {
        CsvOutput::export_rankings_csv(result, "data/step2_rankings.csv");
        CsvOutput::export_head_to_head_csv(result, "data/step2_head_to_head.csv");
        std::cout << "[CSV] Fichiers exportés : data/step2_rankings.csv et data/step2_head_to_head.csv\n";
    }

    return EXIT_SUCCESS;
}

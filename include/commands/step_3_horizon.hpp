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

inline int run_step_3_horizon(const Config& cfg) {
    double expected_rounds = 1.0 / (1.0 - cfg.prob);

    ConsoleOutput::print_section(
        "ÉTAPE 3 (MODIF 2) : HORIZON INCONNU (FIN DU MONDE ALÉATOIRE)\n"
        "  Probabilité de continuation après chaque tour : w = " + std::to_string(cfg.prob) + "\n"
        "  Espérance de durée d'une partie : E[T] = 1 / (1 - w) = " + std::to_string(expected_rounds) + " tours"
    );

    FastRng rng(cfg.seed);
    auto strategies = get_extended_strategies();

    TournamentCfg t_cfg;
    t_cfg.rounds = cfg.rounds;
    t_cfg.matches_per_pair = cfg.matches;
    t_cfg.noise = 0.0;
    t_cfg.continuation_prob = cfg.prob;
    t_cfg.payoff_matrix = PayoffMatrix::standard_pd();
    t_cfg.include_self_play = cfg.self_play;

    TournamentResult result = TournamentEngine::run_tournament(strategies, t_cfg, rng);

    ChartOutput::generate_rankings_svg(
        result,
        "Étape 3 : Classement Horizon Inconnu (w = 0.995)",
        "assets/step3_horizon_rankings.svg"
    );

    ConsoleOutput::print_rankings(
        result,
        "CLASSEMENT HORIZON INCONNU (w = " + std::to_string(cfg.prob) + ", E[T] = " + std::to_string(static_cast<int>(expected_rounds)) + ")"
    );

    if (cfg.export_csv) {
        CsvOutput::export_rankings_csv(result, "data/step3_horizon_rankings.csv");
        std::cout << "[CSV] Fichier exporté : data/step3_horizon_rankings.csv\n";
    }

    return EXIT_SUCCESS;
}

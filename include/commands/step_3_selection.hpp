#pragma once

#include <cstdlib>
#include <iostream>

#include "../strategies/strategies.hpp"
#include "../shared/config.hpp"
#include "../utils/fast_rng.hpp"
#include "../tournament.hpp"
#include "../evolution.hpp"
#include "../output/console_output.hpp"
#include "../output/csv_output.hpp"
#include "../output/chart_output.hpp"

inline int run_step_3_selection(const Config& cfg) {
    ConsoleOutput::print_section(
        "ÉTAPE 3 (MODIF 3) : SÉLECTION ÉVOLUTIONNAIRE (DYNAMIQUE DES RÉPLICATEURS)\n"
        "  Évolution multi-générationnelle des fréquences de la population selon le score"
    );

    FastRng rng(cfg.seed);
    auto strategies = get_extended_strategies();

    TournamentCfg t_cfg;
    t_cfg.rounds = cfg.rounds;
    t_cfg.matches_per_pair = cfg.matches;
    t_cfg.noise = 0.0;
    t_cfg.continuation_prob = std::nullopt;
    t_cfg.payoff_matrix = PayoffMatrix::standard_pd();
    t_cfg.include_self_play = true;

    auto history = EvolutionEngine::simulate(strategies, t_cfg, 50, rng);
    ChartOutput::generate_evolution_svg(history, "assets/step3_evolution.svg");
    ConsoleOutput::print_evolution_history(history);

    if (cfg.export_csv) {
        CsvOutput::export_evolution_csv(history, "data/step3_selection_evolution.csv");
        std::cout << "[CSV] Fichier exporté : data/step3_selection_evolution.csv\n";
    }

    return EXIT_SUCCESS;
}

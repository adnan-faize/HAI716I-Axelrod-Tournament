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

inline int run_step_3_noise(const Config& cfg) {
    ConsoleOutput::print_section(
        "ÉTAPE 3 (MODIF 1) : TOURNOI AVEC BRUIT STOCHASTIQUE\n"
        "  Probabilité d'inversion accidentelle de chaque coup : " + std::to_string(cfg.noise * 100.0) + "%"
    );

    FastRng rng(cfg.seed);
    auto strategies = get_extended_strategies();

    TournamentCfg t_cfg;
    t_cfg.rounds = cfg.rounds;
    t_cfg.matches_per_pair = cfg.matches;
    t_cfg.noise = cfg.noise;
    t_cfg.continuation_prob = std::nullopt;
    t_cfg.payoff_matrix = PayoffMatrix::standard_pd();
    t_cfg.include_self_play = cfg.self_play;

    TournamentResult result = TournamentEngine::run_tournament(strategies, t_cfg, rng);

    ChartOutput::generate_rankings_svg(
        result,
        "Étape 3 : Classement sous Bruit Stochastique (5% d'erreurs)",
        "assets/step3_noise_rankings.svg"
    );

    ConsoleOutput::print_rankings(
        result,
        "CLASSEMENT AVEC BRUIT (" + std::to_string(cfg.noise * 100.0) + "% d'erreurs)"
    );

    if (cfg.export_csv) {
        CsvOutput::export_rankings_csv(result, "data/step3_noise_rankings.csv");
        std::cout << "[CSV] Fichier exporté : data/step3_noise_rankings.csv\n";
    }

    return EXIT_SUCCESS;
}

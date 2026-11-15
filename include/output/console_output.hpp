#pragma once
#pragma once

#include "../tournament.hpp"
#include "../evolution.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

class ConsoleOutput {
public:
    static void print_banner(const std::string& title) {
        std::string line(86, '=');
        std::cout << "\n" << line << "\n";
        std::cout << "  " << title << "\n";
        std::cout << line << "\n";
    }

    static void print_section(const std::string& title) {
        std::string line(86, '#');
        std::cout << "\n" << line << "\n";
        std::cout << "  " << title << "\n";
        std::cout << line << "\n";
    }

    static void print_rankings(const TournamentResult& result, const std::string& title) {
        print_banner(title);

        std::cout << "┌──────┬──────────────────┬────────────┬────────────┬────────────┬──────────┬──────────────┐\n";
        std::cout << "│ Rang │ Stratégie        │ Score Tot. │ Moy/Match  │  Moy/Tour  │  Coop %  │  V / N / D   │\n";
        std::cout << "├──────┼──────────────────┼────────────┼────────────┼────────────┼──────────┼──────────────┤\n";

        for (size_t rank = 0; rank < result.stats.size(); ++rank) {
            const auto& stat = result.stats[rank];
            std::string record = std::to_string(stat.wins) + "/" +
                                 std::to_string(stat.ties) + "/" +
                                 std::to_string(stat.losses);

            std::cout << "│ " << std::left << std::setw(4) << (rank + 1) << " │ "
                      << std::setw(16) << stat.name << " │ "
                      << std::right << std::fixed << std::setprecision(1) << std::setw(10) << stat.total_score << " │ "
                      << std::setprecision(2) << std::setw(10) << stat.avg_score_per_match() << " │ "
                      << std::setprecision(3) << std::setw(10) << stat.avg_score_per_round() << " │ "
                      << std::setprecision(1) << std::setw(7) << stat.cooperation_rate() << "% │ "
                      << std::setw(12) << record << " │\n";
        }
        std::cout << "└──────┴──────────────────┴────────────┴────────────┴────────────┴──────────┴──────────────┘\n\n";
    }

    static void print_head_to_head(const TournamentResult& result) {
        const auto& names = result.strategy_names;
        size_t n = names.size();

        std::cout << "Matrice des gains en tête-à-tête (Score moyen de la ligne face à la colonne) :\n";
        std::cout << std::left << std::setw(16) << "Ligne vs Col";
        for (const auto& name : names) {
            std::string short_name = (name.length() > 8) ? name.substr(0, 8) : name;
            std::cout << " │ " << std::right << std::setw(8) << short_name;
        }
        std::cout << "\n" << std::string(16 + n * 11, '-') << "\n";

        for (size_t i = 0; i < n; ++i) {
            std::string short_row = (names[i].length() > 14) ? names[i].substr(0, 14) : names[i];
            std::cout << std::left << std::setw(16) << short_row;
            for (size_t j = 0; j < n; ++j) {
                std::cout << " │ " << std::right << std::fixed << std::setprecision(1) << std::setw(8) << result.head_to_head[i][j];
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    static void print_match_trace(const TournamentResult& result, const std::string& strat1, const std::string& strat2, size_t max_rounds = 20) {
        for (const auto& m : result.all_match_results) {
            if ((m.name1 == strat1 && m.name2 == strat2) || (m.name1 == strat2 && m.name2 == strat1)) {
                std::cout << "\nTrace d'une confrontation " << strat1 << " vs " << strat2 << " (premiers " << max_rounds << " tours) :\n";
                size_t n = std::min(m.history1.size(), max_rounds);
                std::cout << "  " << std::left << std::setw(16) << m.name1 << " : ";
                for (size_t i = 0; i < n; ++i) {
                    std::cout << to_char(m.history1[i]) << " ";
                }
                std::cout << "\n";
                std::cout << "  " << std::left << std::setw(16) << m.name2 << " : ";
                for (size_t i = 0; i < n; ++i) {
                    std::cout << to_char(m.history2[i]) << " ";
                }
                std::cout << "\n";
                std::cout << std::fixed << std::setprecision(1);
                std::cout << "  Score final du match: " << m.name1 << " = " << m.score1 << ", " << m.name2 << " = " << m.score2 << "\n\n";
                return;
            }
        }
    }

    static void print_evolution_history(const std::vector<GenerationRecord>& history) {
        if (history.empty()) return;

        std::vector<std::string> strat_names;
        for (const auto& [n, _] : history[0].shares) {
            strat_names.push_back(n);
        }

        print_banner("Dynamique des Réplicateurs (Sélection Évolutionnaire)");

        std::cout << std::left << std::setw(8) << "Gén.";
        for (const auto& name : strat_names) {
            std::string short_name = (name.length() > 9) ? name.substr(0, 9) : name;
            std::cout << " │ " << std::right << std::setw(9) << short_name;
        }
        std::cout << " │ " << std::right << std::setw(10) << "Fitness" << "\n";
        std::cout << std::string(95, '-') << "\n";

        size_t total_gens = history.size() - 1;
        for (const auto& record : history) {
            size_t g = record.generation;
            if (g <= 5 || g % 10 == 0 || g == total_gens) {
                std::ostringstream ss;
                ss << "G" << std::setw(3) << std::setfill('0') << g;
                std::cout << std::left << std::setw(8) << ss.str();
                for (const auto& [_, share] : record.shares) {
                    std::cout << " │ " << std::right << std::fixed << std::setprecision(1) << std::setw(8) << (share * 100.0) << "%";
                }
                std::cout << " │ " << std::right << std::fixed << std::setprecision(2) << std::setw(10) << record.avg_fitness << "\n";
            }
        }
        std::cout << std::string(95, '=') << "\n";

        // Visual bar chart for the final generation
        const auto& last = history.back();
        std::cout << "\nRépartition finale de la population (Génération " << last.generation << ") :\n";
        for (const auto& [name, share] : last.shares) {
            double pct = share * 100.0;
            size_t bar_len = static_cast<size_t>(std::round(pct / 2.5));
            std::string bar;
            for (size_t b = 0; b < bar_len; ++b) {
                bar += "█";
            }
            std::cout << "  " << std::left << std::setw(18) << name << " ["
                      << std::right << std::fixed << std::setprecision(1) << std::setw(5) << pct << "%] "
                      << bar << "\n";
        }
        std::cout << "\n";
    }

    static void print_strategies_info(const std::vector<std::unique_ptr<Strategy>>& strategies) {
        print_banner("Fiches Détaillées des Stratégies");

        for (const auto& s : strategies) {
            std::cout << "\n* [" << s->name() << "]\n";
            std::cout << "  - Règle   : " << s->description() << "\n";
            std::cout << "  - Origine : " << s->author_origin() << "\n";
            std::cout << "  - Analyse : " << s->expected_behavior() << "\n";
        }
        std::cout << "\n" << std::string(86, '=') << "\n\n";
    }
};

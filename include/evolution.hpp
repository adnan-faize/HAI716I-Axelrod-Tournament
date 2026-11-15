#pragma once

#include <vector>
#include <string>
#include <utility>
#include <memory>

#include "strategies/strategy.hpp"
#include "tournament.hpp"

struct GenerationRecord {
    size_t generation = 0;
    std::vector<std::pair<std::string, double>> shares;
    double avg_fitness = 0.0;
};

class EvolutionEngine {
public:
    static std::vector<GenerationRecord> simulate(
        const std::vector<std::unique_ptr<Strategy>>& strategies,
        const TournamentCfg& config,
        size_t generations,
        FastRng& rng
    ) {
        size_t n = strategies.size();
        std::vector<std::string> names;
        names.reserve(n);
        for (const auto& s : strategies) {
            names.push_back(s->name());
        }

        // 1. Empirical payoff matrix W_{i,j} with self-play enabled
        TournamentCfg sim_config = config;
        sim_config.include_self_play = true;

        TournamentResult tourney = TournamentEngine::run_tournament(strategies, sim_config, rng);
        const auto& w = tourney.head_to_head;

        // 2. Uniform initial population distribution x_i(0) = 1/n
        std::vector<double> x(n, 1.0 / static_cast<double>(n));

        std::vector<GenerationRecord> history;
        history.reserve(generations + 1);

        double initial_fitness = calculate_avg_fitness(x, w);
        GenerationRecord g0;
        g0.generation = 0;
        g0.avg_fitness = initial_fitness;
        for (size_t i = 0; i < n; ++i) {
            g0.shares.emplace_back(names[i], x[i]);
        }
        history.push_back(std::move(g0));

        for (size_t g_idx = 1; g_idx <= generations; ++g_idx) {
            std::vector<double> fitness(n, 0.0);
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < n; ++j) {
                    fitness[i] += x[j] * w[i][j];
                }
            }

            double avg_f = 0.0;
            for (size_t i = 0; i < n; ++i) {
                avg_f += x[i] * fitness[i];
            }

            if (avg_f <= 1e-9) {
                break;
            }

            std::vector<double> next_x(n, 0.0);
            for (size_t i = 0; i < n; ++i) {
                next_x[i] = x[i] * fitness[i] / avg_f;
                if (next_x[i] < 1e-6) {
                    next_x[i] = 0.0;
                }
            }

            double sum_x = 0.0;
            for (double val : next_x) {
                sum_x += val;
            }
            if (sum_x > 0.0) {
                for (size_t i = 0; i < n; ++i) {
                    next_x[i] /= sum_x;
                }
            }

            x = std::move(next_x);

            GenerationRecord rec;
            rec.generation = g_idx;
            rec.avg_fitness = avg_f;
            for (size_t i = 0; i < n; ++i) {
                rec.shares.emplace_back(names[i], x[i]);
            }
            history.push_back(std::move(rec));
        }

        return history;
    }

private:
    static double calculate_avg_fitness(const std::vector<double>& x, const std::vector<std::vector<double>>& w) {
        size_t n = x.size();
        double total = 0.0;
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                total += x[i] * x[j] * w[i][j];
            }
        }
        return total;
    }
};

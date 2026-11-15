#pragma once

#include "game.hpp"
#include "strategies/strategy.hpp"
#include "utils/fast_rng.hpp"
#include <vector>
#include <string>
#include <optional>
#include <algorithm>
#include <cmath>

struct TournamentCfg {
    uint32_t rounds = 200;
    uint32_t matches_per_pair = 5;
    double noise = 0.0;
    std::optional<double> continuation_prob = std::nullopt;
    PayoffMatrix payoff_matrix = PayoffMatrix::standard_pd();
    bool include_self_play = true;
};

struct MatchResult {
    std::string name1;
    std::string name2;
    double score1 = 0.0;
    double score2 = 0.0;
    uint32_t rounds_played = 0;
    uint32_t cooperations1 = 0;
    uint32_t cooperations2 = 0;
    std::vector<Move> history1;
    std::vector<Move> history2;
};

struct StrategyStats {
    std::string name;
    double total_score = 0.0;
    uint32_t total_rounds = 0;
    uint32_t total_matches = 0;
    uint32_t total_cooperations = 0;
    uint32_t total_defections = 0;
    uint32_t wins = 0;
    uint32_t ties = 0;
    uint32_t losses = 0;

    double avg_score_per_match() const {
        return (total_matches == 0) ? 0.0 : total_score / total_matches;
    }

    double avg_score_per_round() const {
        return (total_rounds == 0) ? 0.0 : total_score / total_rounds;
    }

    double cooperation_rate() const {
        uint32_t total = total_cooperations + total_defections;
        return (total == 0) ? 0.0 : (static_cast<double>(total_cooperations) / total) * 100.0;
    }
};

struct TournamentResult {
    TournamentCfg config;
    std::vector<std::string> strategy_names;
    std::vector<StrategyStats> stats;
    std::vector<std::vector<double>> head_to_head;
    std::vector<MatchResult> all_match_results;
};

class TournamentEngine {
public:
    static MatchResult play_match(
        Strategy& strat1,
        Strategy& strat2,
        const TournamentCfg& config,
        FastRng& rng
    ) {
        strat1.reset();
        strat2.reset();

        std::vector<Move> h1;
        std::vector<Move> h2;

        double score1 = 0.0;
        double score2 = 0.0;
        uint32_t coop1 = 0;
        uint32_t coop2 = 0;
        uint32_t round = 0;

        while (true) {
            round++;

            Move intended1 = strat1.choose(h1, h2, rng);
            Move intended2 = strat2.choose(h2, h1, rng);

            Move actual1 = (config.noise > 0.0 && rng.gen_bool(config.noise)) ? invert(intended1) : intended1;
            Move actual2 = (config.noise > 0.0 && rng.gen_bool(config.noise)) ? invert(intended2) : intended2;

            if (is_cooperation(actual1)) coop1++;
            if (is_cooperation(actual2)) coop2++;

            auto [p1, p2] = config.payoff_matrix.payoffs(actual1, actual2);
            score1 += p1;
            score2 += p2;

            h1.push_back(actual1);
            h2.push_back(actual2);

            if (config.continuation_prob.has_value()) {
                if (!rng.gen_bool(config.continuation_prob.value())) {
                    break;
                }
            } else {
                if (round >= config.rounds) {
                    break;
                }
            }
        }

        MatchResult res;
        res.name1 = strat1.name();
        res.name2 = strat2.name();
        res.score1 = score1;
        res.score2 = score2;
        res.rounds_played = round;
        res.cooperations1 = coop1;
        res.cooperations2 = coop2;
        res.history1 = std::move(h1);
        res.history2 = std::move(h2);
        return res;
    }

    static TournamentResult run_tournament(
        const std::vector<std::unique_ptr<Strategy>>& strategies,
        const TournamentCfg& config,
        FastRng& rng
    ) {
        size_t n = strategies.size();
        std::vector<std::string> names;
        names.reserve(n);
        for (const auto& s : strategies) {
            names.push_back(s->name());
        }

        std::vector<StrategyStats> stats(n);
        for (size_t i = 0; i < n; ++i) {
            stats[i].name = names[i];
        }

        std::vector<std::vector<double>> head_to_head_sum(n, std::vector<double>(n, 0.0));
        std::vector<std::vector<uint32_t>> head_to_head_matches(n, std::vector<uint32_t>(n, 0));
        std::vector<MatchResult> all_match_results;

        for (size_t i = 0; i < n; ++i) {
            size_t start_j = config.include_self_play ? i : (i + 1);
            for (size_t j = start_j; j < n; ++j) {
                for (uint32_t m = 0; m < config.matches_per_pair; ++m) {
                    auto s1 = strategies[i]->clone();
                    auto s2 = strategies[j]->clone();

                    MatchResult result = play_match(*s1, *s2, config, rng);

                    if (i == j) {
                        stats[i].total_score += result.score1;
                        stats[i].total_rounds += result.rounds_played;
                        stats[i].total_matches += 1;
                        stats[i].total_cooperations += result.cooperations1;
                        stats[i].total_defections += (result.rounds_played - result.cooperations1);

                        if (std::abs(result.score1 - result.score2) < 1e-6) {
                            stats[i].ties += 1;
                        } else if (result.score1 > result.score2) {
                            stats[i].wins += 1;
                        } else {
                            stats[i].losses += 1;
                        }

                        head_to_head_sum[i][i] += result.score1;
                        head_to_head_matches[i][i] += 1;
                    } else {
                        stats[i].total_score += result.score1;
                        stats[i].total_rounds += result.rounds_played;
                        stats[i].total_matches += 1;
                        stats[i].total_cooperations += result.cooperations1;
                        stats[i].total_defections += (result.rounds_played - result.cooperations1);

                        stats[j].total_score += result.score2;
                        stats[j].total_rounds += result.rounds_played;
                        stats[j].total_matches += 1;
                        stats[j].total_cooperations += result.cooperations2;
                        stats[j].total_defections += (result.rounds_played - result.cooperations2);

                        if (std::abs(result.score1 - result.score2) < 1e-6) {
                            stats[i].ties += 1;
                            stats[j].ties += 1;
                        } else if (result.score1 > result.score2) {
                            stats[i].wins += 1;
                            stats[j].losses += 1;
                        } else {
                            stats[i].losses += 1;
                            stats[j].wins += 1;
                        }

                        head_to_head_sum[i][j] += result.score1;
                        head_to_head_matches[i][j] += 1;

                        head_to_head_sum[j][i] += result.score2;
                        head_to_head_matches[j][i] += 1;
                    }

                    all_match_results.push_back(std::move(result));
                }
            }
        }

        std::vector<std::vector<double>> head_to_head(n, std::vector<double>(n, 0.0));
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                uint32_t cnt = head_to_head_matches[i][j];
                if (cnt > 0) {
                    head_to_head[i][j] = head_to_head_sum[i][j] / cnt;
                }
            }
        }

        std::sort(stats.begin(), stats.end(), [](const StrategyStats& a, const StrategyStats& b) {
            return a.total_score > b.total_score;
        });

        TournamentResult tourney;
        tourney.config = config;
        tourney.strategy_names = std::move(names);
        tourney.stats = std::move(stats);
        tourney.head_to_head = std::move(head_to_head);
        tourney.all_match_results = std::move(all_match_results);
        return tourney;
    }
};

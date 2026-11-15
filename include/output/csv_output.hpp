#pragma once

#include "../tournament.hpp"
#include "../evolution.hpp"
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <iomanip>
#include <iostream>

class CsvOutput {
public:
    static bool ensure_data_dir(const std::string& dir = "data") {
        try {
            std::filesystem::create_directories(dir);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Erreur lors de la création du dossier '" << dir << "' : " << e.what() << std::endl;
            return false;
        }
    }

    static bool export_rankings_csv(const TournamentResult& result, const std::string& filepath) {
        std::filesystem::path p(filepath);
        if (p.has_parent_path()) {
            ensure_data_dir(p.parent_path().string());
        }

        std::ofstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Impossible d'ouvrir le fichier CSV : " << filepath << std::endl;
            return false;
        }

        file << "Rang,Strategie,ScoreTotal,MoyMatch,MoyTour,CoopPourcent,Victoires,Nuls,Defaites\n";
        for (size_t rank = 0; rank < result.stats.size(); ++rank) {
            const auto& stat = result.stats[rank];
            file << (rank + 1) << ","
                 << "\"" << stat.name << "\","
                 << std::fixed << std::setprecision(1) << stat.total_score << ","
                 << std::setprecision(2) << stat.avg_score_per_match() << ","
                 << std::setprecision(3) << stat.avg_score_per_round() << ","
                 << std::setprecision(1) << stat.cooperation_rate() << ","
                 << stat.wins << ","
                 << stat.ties << ","
                 << stat.losses << "\n";
        }

        file.close();
        return true;
    }

    static bool export_head_to_head_csv(const TournamentResult& result, const std::string& filepath) {
        std::filesystem::path p(filepath);
        if (p.has_parent_path()) {
            ensure_data_dir(p.parent_path().string());
        }

        std::ofstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Impossible d'ouvrir le fichier CSV : " << filepath << std::endl;
            return false;
        }

        const auto& names = result.strategy_names;
        size_t n = names.size();

        file << "Strategie";
        for (const auto& name : names) {
            file << ",\"" << name << "\"";
        }
        file << "\n";

        for (size_t i = 0; i < n; ++i) {
            file << "\"" << names[i] << "\"";
            for (size_t j = 0; j < n; ++j) {
                file << "," << std::fixed << std::setprecision(1) << result.head_to_head[i][j];
            }
            file << "\n";
        }

        file.close();
        return true;
    }

    static bool export_evolution_csv(const std::vector<GenerationRecord>& history, const std::string& filepath) {
        if (history.empty()) return false;

        std::filesystem::path p(filepath);
        if (p.has_parent_path()) {
            ensure_data_dir(p.parent_path().string());
        }

        std::ofstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Impossible d'ouvrir le fichier CSV : " << filepath << std::endl;
            return false;
        }

        file << "Generation,Fitness";
        for (const auto& [name, _] : history[0].shares) {
            file << ",\"" << name << "\"";
        }
        file << "\n";

        for (const auto& record : history) {
            file << record.generation << ","
                 << std::fixed << std::setprecision(2) << record.avg_fitness;
            for (const auto& [_, share] : record.shares) {
                file << "," << std::fixed << std::setprecision(2) << (share * 100.0);
            }
            file << "\n";
        }

        file.close();
        return true;
    }
};

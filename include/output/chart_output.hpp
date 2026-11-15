#pragma once

#include "../tournament.hpp"
#include "../evolution.hpp"
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
#include <sstream>

class ChartOutput {
public:
    static bool generate_evolution_svg(const std::vector<GenerationRecord>& history, const std::string& output_path) {
        if (history.empty()) return false;

        std::filesystem::path p(output_path);
        if (p.has_parent_path()) {
            std::filesystem::create_directories(p.parent_path());
        }

        std::ofstream out(output_path);
        if (!out.is_open()) return false;

        int width = 900;
        int height = 500;
        int margin_left = 70;
        int margin_right = 160;
        int margin_top = 50;
        int margin_bottom = 60;

        int plot_w = width - margin_left - margin_right;
        int plot_h = height - margin_top - margin_bottom;

        size_t num_gens = history.size() - 1;
        std::vector<std::string> strat_names;
        for (const auto& [n, _] : history[0].shares) {
            strat_names.push_back(n);
        }

        const std::vector<std::string> colors = {
            "#e6194b", "#3cb44b", "#ffe119", "#4363d8", "#f58231",
            "#911eb4", "#46f0f0", "#f032e6", "#bcf60c", "#008080"
        };

        out << "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 "
            << width << " " << height << "\" width=\"" << width << "\" height=\"" << height
            << "\" style=\"background:#ffffff; font-family:sans-serif;\">\n";

        // Title
        out << "<text x=\"" << (width / 2) << "\" y=\"30\" font-size=\"18\" font-weight=\"bold\" fill=\"#1e293b\" text-anchor=\"middle\">"
            << "Dynamique des Replicateurs - Evolution des Frequences (50 Generations)</text>\n";

        // Grid lines & Y-axis labels
        for (int i = 0; i <= 5; ++i) {
            int y = margin_top + plot_h - (i * plot_h / 5);
            int pct = i * 20;
            out << "<line x1=\"" << margin_left << "\" y1=\"" << y << "\" x2=\""
                << (margin_left + plot_w) << "\" y2=\"" << y << "\" stroke=\"#e2e8f0\" stroke-width=\"1\"/>\n";
            out << "<text x=\"" << (margin_left - 10) << "\" y=\"" << y
                << "\" font-size=\"12\" fill=\"#64748b\" text-anchor=\"end\" alignment-baseline=\"middle\">"
                << pct << "%</text>\n";
        }

        // X-axis generations
        for (size_t g = 0; g <= num_gens; g += 10) {
            int x = margin_left + static_cast<int>(g * plot_w / num_gens);
            out << "<line x1=\"" << x << "\" y1=\"" << (margin_top + plot_h) << "\" x2=\""
                << x << "\" y2=\"" << (margin_top + plot_h + 6) << "\" stroke=\"#cbd5e1\" stroke-width=\"1\"/>\n";
            out << "<text x=\"" << x << "\" y=\"" << (margin_top + plot_h + 20)
                << "\" font-size=\"12\" fill=\"#64748b\" text-anchor=\"middle\">G" << g << "</text>\n";
        }

        // Axes lines
        out << "<line x1=\"" << margin_left << "\" y1=\"" << margin_top << "\" x2=\""
            << margin_left << "\" y2=\"" << (margin_top + plot_h) << "\" stroke=\"#334155\" stroke-width=\"2\"/>\n";
        out << "<line x1=\"" << margin_left << "\" y1=\"" << (margin_top + plot_h) << "\" x2=\""
            << (margin_left + plot_w) << "\" y2=\"" << (margin_top + plot_h) << "\" stroke=\"#334155\" stroke-width=\"2\"/>\n";

        // Draw strategy lines
        for (size_t s_idx = 0; s_idx < strat_names.size(); ++s_idx) {
            std::string color = colors[s_idx % colors.size()];
            std::ostringstream pts;

            for (const auto& record : history) {
                int x = margin_left + static_cast<int>(record.generation * plot_w / num_gens);
                double share = record.shares[s_idx].second;
                int y = margin_top + plot_h - static_cast<int>(std::round(share * plot_h));
                pts << x << "," << y << " ";
            }

            out << "<polyline fill=\"none\" stroke=\"" << color << "\" stroke-width=\"2.5\" points=\""
                << pts.str() << "\"/>\n";

            int leg_y = margin_top + 15 + static_cast<int>(s_idx * 24);
            int leg_x = margin_left + plot_w + 20;
            double final_share = history.back().shares[s_idx].second * 100.0;

            out << "<rect x=\"" << leg_x << "\" y=\"" << (leg_y - 11) << "\" width=\"14\" height=\"14\" rx=\"2\" fill=\"" << color << "\"/>\n";
            out << "<text x=\"" << (leg_x + 22) << "\" y=\"" << leg_y << "\" font-size=\"12\" fill=\"#1e293b\">"
                << strat_names[s_idx] << " (" << std::fixed << std::setprecision(1) << final_share << "%)</text>\n";
        }

        out << "</svg>\n";
        return true;
    }

    static bool generate_rankings_svg(const TournamentResult& result, const std::string& title, const std::string& output_path) {
        std::filesystem::path p(output_path);
        if (p.has_parent_path()) {
            std::filesystem::create_directories(p.parent_path());
        }

        std::ofstream out(output_path);
        if (!out.is_open()) return false;

        int width = 850;
        int bar_height = 28;
        int n = static_cast<int>(result.stats.size());
        int height = 80 + n * (bar_height + 12);
        int margin_left = 170;
        int margin_right = 80;
        int margin_top = 60;

        int plot_w = width - margin_left - margin_right;
        double max_score = 0.0;
        for (const auto& s : result.stats) {
            if (s.total_score > max_score) max_score = s.total_score;
        }
        double norm_max = (max_score > 0.0) ? (max_score * 1.1) : 1.0;

        out << "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 "
            << width << " " << height << "\" width=\"" << width << "\" height=\"" << height
            << "\" style=\"background:#ffffff; font-family:sans-serif;\">\n";

        // Title
        out << "<text x=\"" << (width / 2) << "\" y=\"32\" font-size=\"18\" font-weight=\"bold\" fill=\"#0f172a\" text-anchor=\"middle\">"
            << title << "</text>\n";

        for (int i = 0; i < n; ++i) {
            const auto& stat = result.stats[i];
            int y = margin_top + i * (bar_height + 12);
            int bar_w = static_cast<int>(std::round((stat.total_score / norm_max) * plot_w));

            std::string bar_color;
            if (i == 0) {
                bar_color = "#2563eb";
            } else if (i < 3) {
                bar_color = "#3b82f6";
            } else if (stat.name == "ALL-D") {
                bar_color = "#dc2626";
            } else {
                bar_color = "#64748b";
            }

            out << "<text x=\"" << (margin_left - 12) << "\" y=\"" << (y + bar_height / 2)
                << "\" font-size=\"13\" font-weight=\"600\" fill=\"#1e293b\" text-anchor=\"end\" alignment-baseline=\"middle\">#"
                << (i + 1) << " " << stat.name << "</text>\n";

            out << "<rect x=\"" << margin_left << "\" y=\"" << y << "\" width=\"" << bar_w
                << "\" height=\"" << bar_height << "\" rx=\"4\" fill=\"" << bar_color << "\"/>\n";

            out << "<text x=\"" << (margin_left + bar_w + 8) << "\" y=\"" << (y + bar_height / 2)
                << "\" font-size=\"12\" font-weight=\"600\" fill=\"#334155\" alignment-baseline=\"middle\"> "
                << std::fixed << std::setprecision(1) << stat.total_score << " pts</text>\n";
        }

        out << "</svg>\n";
        return true;
    }
};

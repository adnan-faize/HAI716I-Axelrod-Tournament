#include <cstdlib>
#include <iostream>
#include <string_view>
#include <getopt.h>

#include "include/shared/config.hpp"
#include "include/strategies/strategies.hpp"
#include "include/commands/step_1.hpp"
#include "include/commands/step_2.hpp"
#include "include/commands/step_3_gains.hpp"
#include "include/commands/step_3_horizon.hpp"
#include "include/commands/step_3_noise.hpp"
#include "include/commands/step_3_selection.hpp"

struct Command {
    std::string_view name;
    std::string_view display_name;
    int (*fn)(const Config&);
    std::string_view description;
};

int run_strats(const Config&);
int run_all(const Config&);
int run_tests(const Config&);

const struct Command commands[] = {
    { "etape-1"          , "Étape 1"               , run_step_1          , "Tournoi de base (les 5 strategies du cours)"                                                                                   },
    { "etape-2"          , "Étape 2"               , run_step_2          , "Tournoi élargi (10 stratégies)"                                                                                                },
    { "etape-3-bruit"    , "Étape 3 : Bruit"       , run_step_3_noise    , "Modification : Chaque coup est inversé avec une petite probabilité"                                                            },
    { "etape-3-horizon"  , "Étape 3 : Horizon"     , run_step_3_horizon  , "Modification : Après chaque tour, la partie continue avec une probabilité w au lieu de durer 200 tours"                        },
    { "etape-3-evolution", "Étape 3 : Évolution"   , run_step_3_selection, "Modification : Le tournoi est rejoué sur plusieurs généraions ; la part de chaque stratégie dans la population suit son score" },
    { "etape-3-gains"    , "Étape 3 : Autres Gains", run_step_3_gains    , "Modification : La matrice change ; soit elle reste un dilemme, soit elle devient un autre jeu 2x2 de la section 07 du cours"   },
    { "strategies"       , "Fiches des Stratégies" , run_strats          , "Affiche les fiches descriptives détaillées de chaque stratégie"                                                                },
    { "tout"             , "Toutes les Étapes"     , run_all             , "Exécute toutes les étapes successivement"                                                                                      },
    { "test"             , "Tests Unitaires"       , run_tests           , "Éxécute la suite de tests unitaires"                                                                                           },
};

const char* short_options = "s:r:m:n:p:cNh";
const struct option long_options[] = {
    { "seed"        , required_argument, nullptr, 's' },
    { "rounds"      , required_argument, nullptr, 'r' },
    { "matches"     , required_argument, nullptr, 'm' },
    { "bruit"       , required_argument, nullptr, 'b' },
    { "prob"        , required_argument, nullptr, 'p' },
    { "csv"         , no_argument      , nullptr, 'c' },
    { "no-self-play", no_argument      , nullptr, 'N' },
    { "help"        , no_argument      , nullptr, 'h' },
    { nullptr       , 0                , nullptr, 0   }
};

void help() {
    std::cout << "HAI716I - Projet 2 · Partie 1 : Tournoi de dilemme du prisonnier                                                                                                " << std::endl;
    std::cout << "Auteur : Adnan FAIZE | Langage : C++20                                                                                                                          " << std::endl;
    std::cout << "                                                                                                                                                                " << std::endl;
    std::cout << "UTILISATION :                                                                                                                                                   " << std::endl;
    std::cout << "  ./tournoi <COMMANDE> [OPTIONS]                                                                                                                                " << std::endl;
    std::cout << "                                                                                                                                                                " << std::endl;
    std::cout << "COMMANDES :                                                                                                                                                     " << std::endl;
    std::cout << "  etape-1                Étape 1 : Tournoi de base (les 5 strategies du cours)                                                                                  " << std::endl;
    std::cout << "  etape-2                Étape 2 : Tournoi élargi (10 stratégies)                                                                                               " << std::endl;
    std::cout << "  etape-3-bruit          Étape 3 : Modification - Chaque coup est inversé avec une petite probabilité                                                           " << std::endl;
    std::cout << "  etape-3-horizon        Étape 3 : Modification - Après chaque tour, la partie continue avec une probabilité w au lieu de durer 200 tours                       " << std::endl;
    std::cout << "  etape-3-selection      Étape 3 : Modification - Le tournoi est rejoué sur plusieurs généraions ; la part de chaque stratégie dans la population suit son score" << std::endl;
    std::cout << "  etape-3-gains          Étape 3 : Modification - La matrice change ; soit elle reste un dilemme, soit elle devient un autre jeu 2x2 de la section 07 du cours  " << std::endl;
    std::cout << "  etape-3-gains          Affiche les fiches descriptives détaillées de chaque stratégie                                                                         " << std::endl;
    std::cout << "  etape-3-gains          Affiche les fiches descriptives détaillées de chaque stratégie                                                                         " << std::endl;
    std::cout << "  etape-3-gains          Affiche les fiches descriptives détaillées de chaque stratégie                                                                         " << std::endl;
    std::cout << "                                                                                                                                                                " << std::endl;
    std::cout << "OPTIONS :                                                                                                                                                       " << std::endl;
    std::cout << "  -s, --seed <value>     Graine aléatoire pour la reproductibilité (défaut : 42)                                                                                " << std::endl;
    std::cout << "  -r, --rounds <value>   Graine aléatoire pour la reproductibilité (défaut : 42)                                                                                " << std::endl;
    std::cout << "  -m, --matches <value>  Nombre de parties par paire (défaut : 5)                                                                                               " << std::endl;
    std::cout << "  -b, --bruit <value>    Probabilité de bruit (défaut : 0.05 pour step3-noise)                                                                                  " << std::endl;
    std::cout << "  -p, --prob <value>     Probabilité w de continuation (défaut : 0.995 pour step3-horizon)                                                                      " << std::endl;
    std::cout << "  -N,  --no-self-play    Désactive les matchs d'une stratégie contre son propre clone                                                                           " << std::endl;
    std::cout << "  -h, --help             Affiche cette aide détaillée                                                                                                           " << std::endl;
    std::cout << "                                                                                                                                                                " << std::endl;
    std::cout << "EXEMPLES :                                                                                                                                                      " << std::endl;
    std::cout << "  ./tournoi etape-1                                                                                                                                             " << std::endl;
    std::cout << "  ./tournoi etape-1 --no-self-play                                                                                                                              " << std::endl;
    std::cout << "  ./tournoi etape-3-bruit --bruit 0.02                                                                                                                          " << std::endl;
    std::cout << "  ./tournoi all                                                                                                                                                 " << std::endl;
}

void cmd_help(const Command& cmd) {
    std::cout << "Command : " << cmd.display_name << std::endl;
    std::cout << "  " << cmd.description << std::endl;
    std::cout << "" << std::endl;
    help();
}

int main(int argc, char** argv) {
    Config cfg;
    bool want_help = false;

    int opt;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1) {
        switch (opt) {
            case 's': cfg.seed       = std::stoull(optarg); break;
            case 'r': cfg.rounds     = std::stoul(optarg);  break;
            case 'm': cfg.matches    = std::stoul(optarg);  break;
            case 'n': cfg.noise      = std::stod(optarg);   break;
            case 'p': cfg.prob       = std::stod(optarg);   break;
            case 'c': cfg.export_csv = true;                break;
            case 'N': cfg.self_play  = false;               break;
            case 'h': want_help      = true;                break;
            case '?':                             [[fallthrough]];
            default: help();                  return EXIT_FAILURE;
        }
    }

    // no command given or only --help
    if (optind >= argc) { help(); return want_help ? EXIT_SUCCESS : EXIT_FAILURE; }

    std::string_view command_name = argv[optind];
    for (const Command& cmd : commands) {
        if (cmd.name == command_name) {
            if (want_help) { cmd_help(cmd); return EXIT_SUCCESS; }
            return cmd.fn(cfg);
        }
    }

    std::cerr << "Erreur : commande inconnue '" << command_name << "'." << std::endl;
    help();
    return EXIT_FAILURE;
}

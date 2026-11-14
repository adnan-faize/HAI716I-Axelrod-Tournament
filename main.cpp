#include <cstdlib>
#include <iostream>
#include <string_view>
#include <getopt.h>

#include "include/shared/config.hpp"
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

const struct Command commands[] = {
    { "step1"          , "Step 1"                , run_step_1          , "Tournoi de base" },
    { "step2"          , "Step 2"                , run_step_2          , "Tournoi de base" },
    { "step3-noise"    , "Step 3 Noise"          , run_step_3_noise    , "Tournoi de base" },
    { "step3-horizon"  , "Step 3 Unknown Horizon", run_step_3_horizon  , "Tournoi de base" },
    { "step3-selection", "Step 3 Selection"      , run_step_3_selection, "Tournoi de base" },
    { "step3-gains"    , "Step 3 Other Gains"    , run_step_3_gains    , "Tournoi de base" },
};

const char* short_options = "s:r:m:n:p:Nh";
const struct option long_options[] = {
    { "seed"        , required_argument, nullptr, 's' },
    { "rounds"      , required_argument, nullptr, 'r' },
    { "matches"     , required_argument, nullptr, 'm' },
    { "noise"       , required_argument, nullptr, 'n' },
    { "prob"        , required_argument, nullptr, 'p' },
    { "no-self-play", required_argument, nullptr, 'N' },
    { "help"        , no_argument      , nullptr, 'h' },
    { nullptr       , 0                , nullptr, 0   }
};

void help() {
    std::cout << "Usage : ./tournament <command> [options]" << std::endl;
    std::cout << "                                        " << std::endl;
    std::cout << "Commands :                              " << std::endl;
    std::cout << "  step1             Step 1 : Base tournament                             " << std::endl;
    std::cout << "  step2             Step 2 : Base tournament                             " << std::endl;
    std::cout << "  step3-bruit       Step 3 : Modification 1 - Noise                            " << std::endl;
    std::cout << "  step3-horizon     Step 3 : Modification 2 - Unknown horizon                            " << std::endl;
    std::cout << "  step3-selection   Step 3 : Modification 3 - Selection                             " << std::endl;
    std::cout << "  step3-gains       Step 3 : Modification 4 - Other gains                            " << std::endl;
    std::cout << "                                        " << std::endl;
    std::cout << "Options :                               " << std::endl;
    std::cout << "  -s, --seed <value>    Random seed for reproductibility" << std::endl;
    std::cout << "  -r, --rounds <value>  Number of rounds for ea" << std::endl;
    std::cout << "  -m, --matches <value>  Number of matches per pair" << std::endl;
    std::cout << "  -n, --noise <value>      Noise probability (default : 0.05 for step_3-noise)" << std::endl;
    std::cout << "  -p, --prob <value>                  " << std::endl;
    std::cout << "  -N,  --no-self-play                 " << std::endl;
    std::cout << "  -h, --help    Show this help message" << std::endl;
    std::cout << "                                      " << std::endl;
    std::cout << "Example :                             " << std::endl;
    std::cout << "  ./tournament ...                    " << std::endl;
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
            case 's': cfg.seed    = std::stoull(optarg); break;
            case 'r': cfg.rounds  = std::stoul(optarg);  break;
            case 'm': cfg.matches = std::stoul(optarg);  break;
            case 'n': cfg.noise   = std::atof(optarg);   break;
            case 'p': cfg.prob    = std::atof(optarg);   break;
            case 'N': /* TODO */                         break;
            case 'h': want_help = true;                  break;
            case '?':                          [[fallthrough]];
            default: help();               return EXIT_FAILURE;
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

    std::cerr << "Error : unknown command '" << command_name << "'." << std::endl;
    help();
    return EXIT_FAILURE;
}

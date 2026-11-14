#include <cstdlib>
#include <cstdint>

#include <iostream>

#include <getopt.h>

#include "include/shared/config.hpp"
#include "include/commands/step_1.hpp"


struct Command {
    std::string_view name;
    int (*fn)(const Config&);
    std::string_view description;
};

const struct Command commands[] = {
    { "step1", run_step_1, "Tournoi de base" },
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
    std::cout << "" << std::endl;
    std::cout << "Commands : " << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Options : " << std::endl;
    std::cout << "  -s, --seed <value>    Random seed for reproductibility" << std::endl;
    std::cout << "  -r, --rounds <value>  Number of rounds for ea" << std::endl;
    std::cout << "  -m, --matches <value>  Number of matches per pair" << std::endl;
    std::cout << "  -n, --noise <value>      Noise probability (default : 0.05 for step_3-noise)" << std::endl;
    std::cout << "  -p, --prob <value> " << std::endl;
    std::cout << "  -N,  --no-self-play  " << std::endl;
    std::cout << "  -h, --help    Show this help message" << std::endl;
    std::cout << "" <<std::endl;
    std::cout << "Example : " << std::endl;
    std::cout << "  ./tournament ..." << std::endl;
}

int main(int argc, char** argv) {
    Config cfg;

    int opt;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1) {
        switch (opt) {
            case 's': cfg.seed    = std::stoull(optarg); break;
            case 'r': cfg.rounds  = std::stoul(optarg); break;
            case 'm': cfg.matches = std::stoul(optarg); break;
            case 'n': cfg.noise   = std::atof(optarg); break;
            case 'p': cfg.prob    = std::atof(optarg); break;
            case 'N': break; // TODO
            case 'h':
                help();
                return EXIT_SUCCESS;
            case '?': [[fallthrough]];
            default:
                help();
                return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

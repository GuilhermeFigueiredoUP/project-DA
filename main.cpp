#include <iostream>
#include <string>

#include "include/cli_parser.hpp"
#include "include/solver.hpp"

int main(int argc, char *argv[]) {
    int ret;
    if (argc > 1) {
        ret = parseArguments(argc, argv);
        if (ret != 0) return ret;
        else return 0;
    }
    Solver solver;

    std::cout << "Welcome to DA2026_PRJ1_T16_G1's algorithm" << std::endl << "type -help for more information." << std::endl;
    std::string input;
    bool running = true;

    // main loop for CLI (menu mode)
    while (running) {
        std::cout << "$ ";
        std::getline(std::cin, input);

        if (input == "quit") {
            running = false;
        }
    }
    
    return 0;
}
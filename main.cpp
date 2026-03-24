#include <iostream>
#include <string>

#include "include/cli_parser.hpp"
#include "include/solver.hpp"

int main(int argc, char *argv[]) {
    int ret;
    if (argc > 1) {
        ret = parseArguments(argc, argv);
        if (ret != 0) return ret;
        return 0;
    }
    Solver solver;

    std::cout << "Welcome to DA2026_PRJ1_T16_G1's algorithm" << std::endl << "type -help for more information." << std::endl;
    std::string input;
    bool running = true;
    Solver mySolver;

    // main loop for CLI (menu mode)
    while (running) {
        std::cout << "$ ";
        std::getline(std::cin, input);

        if (input == "quit") {
            running = false;
        }
        else {
            Terminal_cmd(input, mySolver);
        }
    }
    return 0;
}
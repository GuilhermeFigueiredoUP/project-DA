#include <iostream>
#include <string>
#include <vector>
#include "include/cli_parser.hpp"
using namespace std;

int main(int argc, char *argv[]) {

    if (argc > 1) {
        
        if (parseArguments(argc, argv) != 0) return -1;
        
        vector<DataNode> allNodes;
        Parameters params;
        Control ctrl;
        
        if(parseInput(argv[2],allNodes,params,ctrl)!=0)return -1;
        
        return 0;
        
    }

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
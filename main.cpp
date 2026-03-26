#include <iostream>
#include <string>

#include "include/cli_parser.hpp"
#include "include/solver.hpp"
#include "risk_analysis.hpp"

int main(int argc, char *argv[]) {
    int ret;
    std:: vector<DataNode> allNodes;
    Parameters params;
    Control ctrl;
    std::string final_file;
    if (argc > 1) {
        ret = parseArguments(argc, argv, allNodes, params, ctrl);
        if (ret != 0) return ret;
        
        if (argc >=4) final_file = argv[3];
        else{
            final_file = ctrl.OutputFileName.empty()? "output.csv": ctrl.OutputFileName;
        }
        std::cout << "DEBUG MAIN: RiskAnalysis vale " << ctrl.RiskAnalysis << std::endl;
        if (ctrl.RiskAnalysis ==1){
            risk_analysis(allNodes, params, ctrl, final_file);
        }
        return 0;
        
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
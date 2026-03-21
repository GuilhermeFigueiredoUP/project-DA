#include "../include/cli_parser.hpp"
#include "../include/solver.hpp"

#include <iostream>
#include <cstring>

bool hasCsvExtension(const std::string &filename) {
    return filename.size() >= 4 && filename.substr(filename.size() - 4) == ".csv";
}

int parseArguments(int argc, char *argv[]) {
    // check argument count and batch mode flag
    if (argc != 4 || strcmp(argv[1], "-b") != 0) {
        std::cerr << "Correct usage: program -b input.csv output.csv" << std::endl;
        return -1;
    }

    std::string inputFile = argv[2];
    std::string outputFile = argv[3];

    // check .csv extension
    if (!hasCsvExtension(inputFile) || !hasCsvExtension(outputFile)) {
        std::cerr << "Error: files must have .csv extension\n";
        return -1;
    }

    std::cout << "Batch mode: Processing...\n";
    Solver batchSolver;

    batchSolver.updateInputFile(inputFile);
    batchSolver.updateOutputFile(outputFile);

    batchSolver.processInput();
    batchSolver.computeAssignment();
    batchSolver.generateOutput();

    std::cout << "Batch mode: Finished successfully!\n";

    return 0;
}

int parseInput(std::string input, Solver& solver) {
    // Help Command
    if (input == "-help") {
        std::cout << "Available commands:\n";
        std::cout << "  load <filename> - Load the dataset\n";
        std::cout << "  run             - Execute the algorithm\n";
        std::cout << "  quit            - Exit the program\n";
    } 
    // Empty Command
    else if (input.empty()) {
        return 0; 
    } 
    // Quit Command (in main)
    else if (input == "quit") {
        return 0;
    }
    // Load Command
    else if (input.find("load ") == 0) { 
        std::string filename = input.substr(5);
        
        if (filename.empty()) {
            std::cout << "Correct usage: load data.csv\n";
        } else {
            std::cout << "Preparing to load file: " << filename << "\n";
            solver.updateInputFile(filename);
            solver.processInput();
            std::cout << "File loaded.\n";
        }
    }
    else if (input.find("load") == 0) {
        std::cout << "Correct usage: load data.csv\n";
    }
    // Run Command
    else if (input == "run") {
        std::cout << "Starting the assignment process...\n"; //talvez mudar o texto
        solver.computeAssignment();
        solver.generateOutput();
        std::cout << "Algorithm finished!\n";
    }
    // Unknown Command
    else {
        std::cout << "Unknown command. Type '-help' for more information.\n";
    }
    return 0;
}
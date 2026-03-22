#include "../include/cli_parser.hpp"
#include "../include/solver.hpp"

#include <iostream>
#include <cstring>
#include <stdexcept>

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
    if (input == "-help" || input == "help") {
        std::cout << "Available commands:\n";
        std::cout << "  load <filename>       - Load the dataset\n";
        std::cout << "  run                   - Execute the algorithm\n";
        std::cout << "  set_max <number>      - Set max reviews per reviewer\n";
        std::cout << "  set_min <number>      - Set min reviews per submission\n";
        std::cout << "  set_risk <number>     - Set risk analysis level (0, 1, or K)\n";
        std::cout << "  set_gen <0-2>         - Set assignment generation mode\n";
        std::cout << "  set_output <filename> - Set the output filename\n";
        std::cout << "  reset                 - Reset settings to original input\n";
        std::cout << "  quit                  - Exit the program\n";
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
        solver.updateInputFile(filename);
        if (solver.processInput() == 0) {
            std::cout << "File loaded successfully!\n";
        } else {
            std::cout << "Error loading file.\n";
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
    // Set Max Reviews Command
    else if (input.find("set_max ") == 0) {
        std::string numberStr = input.substr(8);
        if (numberStr.find('.') != std::string::npos) {
            std::cout << "Please enter a whole number.\n";
        }
        else {
            try {
                int maxReviews = std::stoi(numberStr);
                if (maxReviews < 0) {
                    std::cout << "Error: The number of reviews cannot be negative!\n";
                }
                else {
                    solver.updateMaxReviewsPerReviewer(maxReviews);
                    std::cout << "Max reviews per reviewer updated to: " << maxReviews << "\n";
                }
            }
            catch (const std::invalid_argument& e) {
                std::cout << "Please enter a number (e.g., set_max 5)\n";
            }
            catch (const std::out_of_range& e) {
                std::cout << "Please enter a smaller number.\n";
            }
        }
    }
    // Set Min Reviews Command
    else if (input.find("set_min ") == 0) {
        std::string numberStr = input.substr(8);
        if (numberStr.find('.') != std::string::npos) {
            std::cout << "Please enter a whole number.\n";
        }
        else {
            try {
                int minReviews = std::stoi(numberStr);
                if (minReviews < 0) {
                    std::cout << "Please enter a positive number\n";
                }
                else {
                    solver.updateMinReviewsPerSubmission(minReviews);
                    std::cout << "Min reviews per submission updated to: " << minReviews << "\n";
                }
            }
            catch (const std::invalid_argument& e) {
                std::cout << "Please enter a valid integer (e.g., set_min 3).\n";
            }
            catch (const std::out_of_range& e) {
                std::cout << "Please enter a smaller number.\n";
            }
        }
    }
    // Set Risk Analysis (0, 1, or K)
    else if (input.find("set_risk ") == 0) {
        std::string numberStr = input.substr(9);
        if (numberStr.find('.') != std::string::npos) std::cout << "Please enter a whole number.\n";
        else {
            try {
                int riskVal = std::stoi(numberStr);
                if (riskVal < 0) std::cout << "Risk analysis cannot be negative.\n";
                else {
                    solver.updateRiskAnalysis(riskVal);
                    std::cout << "Risk Analysis set to: " << riskVal << "\n";
                }
            } catch (...) { std::cout << "Please enter a valid number.\n"; }
        }
    }
    // Set Generate Assignments Mode Command
    else if (input.find("set_gen ") == 0) {
        std::string numberStr = input.substr(8);
        try {
            int genMode = std::stoi(numberStr);
            if (genMode == 0) solver.updateComputeMode(NO_REPORT);
            else if (genMode == 1) solver.updateComputeMode(PRIMARY_ONLY);
            else if (genMode == 2) solver.updateComputeMode(SUBMISSION_SECONDARY);
            else {
                std::cout << "Generate mode must be 0, 1, or 2.\n";
                return 0;
            }
            std::cout << "Assignment generation mode updated!\n";
        } catch (...) { std::cout << "Please enter a valid number: 0-2.\n"; }
    }
    // Set Output File Command
    else if (input.find("set_output ") == 0) {
        std::string filename = input.substr(11);
        if (filename.empty()) {
            std::cout << "Please provide a valid filename (e.g., set_output results.csv)\n";
        } else {
            solver.updateOutputFile(filename);
            std::cout << "Output file set to: " << filename << "\n";
        }
    }
    // Reset Command
    else if (input == "reset") {
        //TODO
        std::cout << "Parameters have been reset to their original values.\n";
    }
    // Unknown Command
    else {
        std::cout << "Unknown command. Type '-help' for more information.\n";
    }
    return 0;
}
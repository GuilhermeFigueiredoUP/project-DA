/**
 * @file cli_parser.cpp
 * @brief recieves input from command line and reads the contents of the input file
 */
#include "../include/cli_parser.hpp"

#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>

bool hasCsvExtension(const std::string &filename) {
    return filename.size() >= 4 && filename.substr(filename.size() - 4) == ".csv";
}

int parseArguments(int argc, char *argv[], Solver &solver) {
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
    int status = parseInput(inputFile, solver);
    if (status != 0) {
        return -1;
    }

    solver.computeAssignment();
    solver.generateOutput();

    return 0;
}
/// @brief removes the unecessary whitespaces in a string
/// @param s input string to process
/// @return string without the whitespaces
std::string remove_espacos(const std::string &s){
    std::string res =s;
    const std::string whitespace= " \t\r\n";
    size_t first =res.find_first_not_of(whitespace);
    if (first == std::string::npos) return ""; 
    res.erase(0,first);
    size_t last= res.find_last_not_of(whitespace);
    if (last != std::string::npos){
        res.erase(last+1);
    }
    return res;
}

/// @brief removes the quotation marks from a string 
/// @param s input string to process 
/// @return string with just it's text (non-whitespaces/quotation marks)
std::string remove_aspas(const std::string &s){
    std::string result = remove_espacos(s);
    if (result[0] =='"' && result[result.size()-1]=='"' ) return result.substr(1,result.size()-2);
    return result; // se p qlq motivo n tiver aspas
}

int parseInput(std::string inputFile, Solver &solver) {
    std::ifstream file(inputFile);
    if (!file.is_open()){
        std::cerr <<"Error: not possible to open file "<<inputFile<<std::endl;
        return -1;
    }
    
    std::string current_section = "";
    std::string line;
    std::vector<DataNode> submissionNodes;
    std::vector<DataNode> reviewerNodes;
    uint8_t parameterFlags = 0x00;
    

    while (getline(file, line)) {
        line=remove_espacos(line);

        if (line.empty() || line == "#" || line == "\r") continue; //ignoro linhas vazias

        //  se a linha começa com #, atualiza a secção e pula para a próxima linha
        if (line[0] == '#') {
            if (line.find("#Submissions") != std::string::npos) current_section = "submissions";
            else if (line.find("#Reviewers") != std::string::npos) current_section = "reviewers";
            else if (line.find("#Parameters") != std::string::npos) current_section = "parameters";
            else if (line.find("#Control") != std::string::npos) current_section = "control";
            continue; 
        }


        std::stringstream ss(line);
        std::string field;

        if (current_section == "submissions") {
            DataNode s;
            s.type =SUBMISSION;
            getline(ss, field, ','); s.id = stoi(remove_espacos(field)); // stoi converte string em int
            getline(ss, field, ','); s.nameTitle = remove_aspas(field);
            getline(ss, field, ','); s.authors = remove_espacos(field);
            getline(ss, field, ','); s.email = remove_espacos(field);
            getline(ss, field, ','); s.primaryDomain = stoi(remove_espacos(field));
            
            s.secondaryDomain=-1;
            if (getline(ss, field, ',')){
                std::string f = remove_espacos(field);
                if (!f.empty()) s.secondaryDomain=stoi(f);
            }
            std::cout << "adding submission with id: " << s.id << " to reviewers" << std::endl;

            submissionNodes.push_back(s);

        }

        else if (current_section == "reviewers") {
            DataNode r;
            r.type = REVIEWER;
            getline(ss, field, ','); r.id = stoi(remove_espacos(field));
            getline(ss, field, ','); r.nameTitle = remove_espacos(field);
            getline(ss, field, ','); r.email = remove_espacos(field);
            getline(ss, field, ','); r.primaryDomain = stoi(remove_espacos(field));
            
            r.secondaryDomain=-1;
            if (getline(ss, field, ',')){
                std::string f = remove_espacos(field);
                if (!f.empty()) r.secondaryDomain=stoi(f);
            }
            std::cout << "adding reviewer with id: " << r.id << " to reviewers" << std::endl;

            reviewerNodes.push_back(r);

        }

        else if (current_section == "parameters") {
            std::string key, value;
            getline(ss, key, ','); key = remove_espacos(key);
            getline(ss, value, ','); value = remove_espacos(value);
            
            
            if (key== "MinReviewsPerSubmission") solver.updateMinReviewsPerSubmission(stoi(value));
            else if (key== "MaxReviewsPerReviewer") solver.updateMaxReviewsPerReviewer(stoi(value));
            else if (key== "PrimaryReviewerExpertise") parameterFlags = (stoi(value))? parameterFlags | PRIMARY_REVIEWER_EXPERTISE : parameterFlags;
            else if (key== "SecondaryReviewerExpertise") parameterFlags = (stoi(value))? parameterFlags | SECONDARY_REVIEWER_EXPERTISE : parameterFlags;
            else if (key== "PrimarySubmissionDomain") parameterFlags = (stoi(value))? parameterFlags | PRIMARY_SUBMISSION_DOMAIN : parameterFlags;
            else if (key== "SecondarySubmissionDomain") parameterFlags = (stoi(value))? parameterFlags | SECONDARY_SUBMISSION_DOMAIN : parameterFlags;
            else std::cerr <<"Warning: unknown parameter: "<< key<< std::endl; 

        }
        else if (current_section == "control") {
            std::string key,value;
            getline(ss, key, ','); key = remove_espacos(key);
            getline(ss, value, ','); value = remove_espacos(value);
            
        
            if (key== "GenerateAssignments") solver.updateComputeMode((ComputeMode)stoi(value));
            //std::cout << "[PARSER DEBUG] Li o valor: " << value << std::endl;
            if (key== "RiskAnalysis") solver.updateRiskAnalysis(stoi(value));
            if (key== "OutputFileName") solver.updateOutputFile(remove_aspas(value));
            
            else std::cerr <<"Warning: unknown control parameter: "<< key<< std::endl; 

        }

        solver.setReviewers(reviewerNodes);
        solver.setSubmissions(submissionNodes);
        solver.updateParameterFlags(parameterFlags);
    }
    return 0;
}


int Terminal_cmd(std::string input, Solver& solver) {
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
    // Load Command
    else if (input.find("load ") == 0) {
        std::string filename = input.substr(5);
        solver.updateInputFile(filename);
        if (parseInput(filename, solver) == 0) {
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
    // Set Risk Analysis Command
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
        std::string currentFile = solver.getInputFile();

    }
}
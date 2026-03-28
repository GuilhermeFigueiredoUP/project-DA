/**
 * @file cli_parser.cpp
 * @brief recieves input from command line and reads the contents of the input file
 */
#include "../include/cli_parser.hpp"

#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
/// @brief verifies if the file has ".csv" extension
///@note case sensitive  
/// @param filename name of the file
/// @return true if file ends in ".csv" and false if not
bool hasCsvExtension(const std::string &filename) {
    return filename.size() >= 4 && filename.substr(filename.size() - 4) == ".csv";
}



int parseArguments(int argc, char *argv[], std:: vector<DataNode>& allNodes, Parameters& params, Control& ctrl) {
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
    int status = parseInput(inputFile,allNodes,params,ctrl);
    if (status != 0) {
        return -1;
    }
    

    return 0;
}
/// @brief removes the unecessary whitespaces in a string
/// @param s input string to process
/// @return string without the whitespaces
string remove_espaços(const std::string &s){
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
    std::string result = remove_espaços(s);
    if (result[0] =='"' && result[result.size()-1]=='"' ) return result.substr(1,result.size()-2);
    return result; // se p qlq motivo n tiver aspas
}

int parseInput(string inputFile, vector<DataNode>& allNodes, Parameters &params, Control&ctrl) {
    ifstream file(inputFile);
    if (!file.is_open()){
        cerr <<"Error: not possible to open file "<<inputFile<<endl;
        return -1;
    }
    
    string current_section = "";
    string line;
    

    while (getline(file, line)) {
        line=remove_espaços(line);

        if (line.empty() || line == "#" || line == "\r") continue; //ignoro linhas vazias

        //  se a linha começa com #, atualiza a secção e pula para a próxima linha
        if (line[0] == '#') {
            if (line.find("#Submissions") != string::npos) current_section = "submissions";
            else if (line.find("#Reviewers") != string::npos) current_section = "reviewers";
            else if (line.find("#Parameters") != string::npos) current_section = "parameters";
            else if (line.find("#Control") != string::npos) current_section = "control";
            continue; 
        }


        stringstream ss(line);
        string field;

        if (current_section == "submissions") {
            DataNode s;
            s.type =SUBMISSION;
            getline(ss, field, ','); s.id = stoi(remove_espaços(field)); // stoi converte string em int
            getline(ss, field, ','); s.nameTitle = remove_aspas(field);
            getline(ss, field, ','); s.authors = remove_espaços(field);
            getline(ss, field, ','); s.email = remove_espaços(field);
            getline(ss, field, ','); s.primaryDomain = stoi(remove_espaços(field));
            
            s.secondaryDomain=-1;
            if (getline(ss, field, ',')){
                string f = remove_espaços(field);
                if (!f.empty()) s.secondaryDomain=stoi(f);
            }

            allNodes.push_back(s);

        }

        else if (current_section == "reviewers") {
            DataNode r;
            r.type = REVIEWER;
            getline(ss, field, ','); r.id = stoi(remove_espaços(field));
            getline(ss, field, ','); r.nameTitle = remove_espaços(field);
            getline(ss, field, ','); r.email = remove_espaços(field);
            getline(ss, field, ','); r.primaryDomain = stoi(remove_espaços(field));
            
            r.secondaryDomain=-1;
            if (getline(ss, field, ',')){
                string f = remove_espaços(field);
                if (!f.empty()) r.secondaryDomain=stoi(f);
            }

            allNodes.push_back(r);

        }

        else if (current_section == "parameters") {
            string key,value;
            getline(ss, key, ','); key = remove_espaços(key);
            getline(ss, value, ','); value = remove_espaços(value);
            
            
            if (key== "MinReviewsPerSubmission") params.MinReviewsPerSubmission=stoi(value);
            else if (key== "MaxReviewsPerReviewer") params.MaxReviewsPerReviewer=stoi(value);
            else if (key== "PrimaryReviewerExpertise") params.PrimaryReviewerExpertise=stoi(value);
            else if (key== "SecondaryReviewerExpertise") params.SecondaryReviewerExpertise=stoi(value);
            else if (key== "PrimarySubmissionDomain") params.PrimarySubmissionDomain=stoi(value);
            else if (key== "SecondarySubmissionDomain") params.SecondarySubmissionDomain=stoi(value);
            else cerr <<"Warning: unknown parameter: "<< key<< endl; 

        }
        else if (current_section == "control") {
            string key,value;
            getline(ss, key, ','); key = remove_espaços(key);
            getline(ss, value, ','); value = remove_espaços(value);
            
        
            if (key== "GenerateAssignments") ctrl.GenerateAssignments=stoi(value);
            std::cout << "[PARSER DEBUG] Li o valor: " << value << std::endl;
            if (key== "RiskAnalysis") ctrl.RiskAnalysis=stoi(value);
            if (key== "OutputFileName") ctrl.OutputFileName=remove_aspas(value);
            
            else cerr <<"Warning: unknown control parameter: "<< key<< endl; 

        }
    }
    return 0;
}


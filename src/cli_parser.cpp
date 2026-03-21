#include "../include/cli_parser.hpp"

#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

bool hasCsvExtension(const string &filename) {
    return filename.size() >= 4 && filename.substr(filename.size() - 4) == ".csv";
}

int parseArguments(int argc, char *argv[]) {
    // check argument count and batch mode flag
    if (argc != 4 || strcmp(argv[1], "-b") != 0) {
        cerr << "Correct usage: program -b input.csv output.csv" << endl;
        return -1;
    }

    string inputFile = argv[2];
    string outputFile = argv[3];

    // check .csv extension
    if (!hasCsvExtension(inputFile) || !hasCsvExtension(outputFile)) {
        cerr << "Error: files must have .csv extension\n";
        return -1;
    }

    return 0;
}


string remove_espaços( string s){
    const string whitespace= " \t\r\n";
    s.erase(0, s.find_first_not_of(whitespace)); // apaga de 'a' a 'b'
    size_t last= s.find_last_not_of(whitespace);
    if (last != string::npos){
        s.erase(last+1);
    }
    return s;
}

string remove_aspas(const string &s){
    string result = remove_espaços(s);
    if (result[0] =='"' && result[result.size()-1]=='"' ) return result.substr(1,result.size()-2);
    return result; // se p qlq motivo n tiver aspas
}

int parseInput(string inputFile, vector<Submission>& allSubmissions, vector<Reviewer>& allReviewers, Parameters &params, Control&ctrl) {
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
            Submission s;
            getline(ss, field, ','); s.id = stoi(remove_espaços(field)); // stoi converte string em int
            getline(ss, field, ','); s.title = remove_aspas(field);
            getline(ss, field, ','); s.authors = remove_espaços(field);
            getline(ss, field, ','); s.email = remove_espaços(field);
            getline(ss, field, ','); s.primary_topic = stoi(remove_espaços(field));
            
            s.secondary_topic=-1;
            if (getline(ss, field, ',')){
                string f = remove_espaços(field);
                if (!f.empty()) s.secondary_topic=stoi(f);
            }

            allSubmissions.push_back(s);

        }

        else if (current_section == "reviewers") {
            Reviewer r;
            getline(ss, field, ','); r.id = stoi(remove_espaços(field));
            getline(ss, field, ','); r.name = remove_espaços(field);
            getline(ss, field, ','); r.email = remove_espaços(field);
            getline(ss, field, ','); r.primary_topic = stoi(remove_espaços(field));
            
            r.secondary_topic=-1;
            if (getline(ss, field, ',')){
                string f = remove_espaços(field);
                if (!f.empty()) r.secondary_topic=stoi(f);
            }

            allReviewers.push_back(r);

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
            else if (key== "RiskAnalysis") ctrl.RiskAnalysis=stoi(value);
            else if (key== "OutputFileName") ctrl.OutputFileName=remove_aspas(value);
            
            else cerr <<"Warning: unknown control parameter: "<< key<< endl; 

        }
    }
    return 0;
}

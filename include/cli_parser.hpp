#pragma once
#include <vector>
#include <string>
using namespace std;

struct Submission {
        int id;
        string title;
        string authors;
        string email;
        int primary_topic;
        int secondary_topic; //-1 se nao existir
    };

struct Reviewer { 
    int id;
    string name;
    string email;
    int primary_topic;
    int secondary_topic;
};

struct Parameters{
    int MinReviewsPerSubmission;
    int MaxReviewsPerReviewer;
    int PrimaryReviewerExpertise;
    int SecondaryReviewerExpertise;
    int PrimarySubmissionDomain;
    int SecondarySubmissionDomain;
};

struct Control{
    int GenerateAssignments;
    int RiskAnalysis;
    string OutputFileName;
};


// parser for batch mode execution
int parseArguments(int argc, char *argv[]);

// parser for menu mode execution
int parseInput(string inputFile, vector<Submission>& allSubmissions, vector<Reviewer>& allReviewers, Parameters &params, Control&ctrl);

// --- helper methods ---
bool hasCsvExtension(const std::string &filename);
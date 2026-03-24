#pragma once
#include <vector>
#include <string>
using namespace std; 
enum DataType {
    SUBMISSION,
    REVIEWER,
    SOURCE,
    SINK
};

struct DataNode {
    DataType type;
    int id;
    int primaryDomain;
    int secondaryDomain;
    std::string email;
    std::string nameTitle;
    std::string authors;

    bool operator==(DataNode other) {
        if(id != other.id) return false;
        return true;
    }
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
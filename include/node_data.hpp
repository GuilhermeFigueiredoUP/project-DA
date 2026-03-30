#pragma once
#include <vector>
#include <string>

/// @brief Enum to represent the type of data node (submission, reviewer, source, sink)
enum DataType {
    SUBMISSION,
    REVIEWER,
    SOURCE,
    SINK
};

/// @brief Struct to represent a data node in the graph, which can be a submission, reviewer, source, or sink
/// @param type The type of the data node (submission, reviewer, source, sink)
/// @param id The unique identifier for the data node
/// @param primaryDomain The primary domain associated with the data node (for submissions and reviewers)
/// @param secondaryDomain The secondary domain associated with the data node (for submissions and reviewers)
/// @param email The email address associated with the data node (for reviewers)
/// @param nameTitle The name or title associated with the data node (for submissions and reviewers)
/// @param authors The authors associated with the data node (for submissions)
/// @note The equality operator is defined to compare data nodes based on their type and id, ignoring other fields
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
        if(type != other.type) return false;
        return true;
    }
};

/// @brief Struct to represent the parameters for the solver, including review requirements and matching criteria
/// @param MinReviewsPerSubmission The minimum number of reviews required per submission
/// @param MaxReviewsPerReviewer The maximum number of reviews allowed per reviewer
/// @param PrimaryReviewerExpertise Flag indicating if primary reviewer expertise should be considered in matching
/// @param SecondaryReviewerExpertise Flag indicating if secondary reviewer expertise should be considered in matching
/// @param PrimarySubmissionDomain Flag indicating if primary submission domain should be considered in matching
/// @param SecondarySubmissionDomain Flag indicating if secondary submission domain should be considered in matching
struct Parameters{
int MinReviewsPerSubmission;
int MaxReviewsPerReviewer;
int PrimaryReviewerExpertise;
int SecondaryReviewerExpertise;
int PrimarySubmissionDomain;
int SecondarySubmissionDomain;
};

/// @brief Struct to represent control settings for the solver, including whether to generate assignments and perform risk analysis
/// @param GenerateAssignments Flag indicating whether to generate the final assignment output file
/// @param RiskAnalysis Flag indicating whether to perform risk analysis on the generated assignments
/// @param OutputFileName The name of the output file to generate (if GenerateAssignments is set)
struct Control{
    int GenerateAssignments;
    int RiskAnalysis;
    std::string OutputFileName;
};
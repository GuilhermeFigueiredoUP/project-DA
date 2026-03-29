#pragma once
#include <cstdint>
#include <string>
#include <cstdint>

#include "../data_structures/Graph.h"
#include "../include/node_data.hpp"

#define BIT(n) (1<<n)
// parameter flag values
#define PRIMARY_REVIEWER_EXPERTISE BIT(0)
#define SECONDARY_REVIEWER_EXPERTISE BIT(1)
#define PRIMARY_SUBMISSION_DOMAIN BIT(2)
#define SECONDARY_SUBMISSION_DOMAIN BIT(3)

// control flag values
#define GENERATE_ASSIGNMENTS BIT(0)
#define RISK_ANALYSIS BIT(1)

enum ComputeMode {
    NO_REPORT,
    PRIMARY_ONLY,
    SUBMISSION_SECONDARY,
    ALL
};

class Solver {
    public:
    Solver();

    int processInput();
    int generateOutput();
    int computeAssignment();
    void processDummyInput(); // hard coded data for development

    // file handling
    void updateInputFile(std::string filePath);
    void updateOutputFile(std::string filePath);
    std::string getInputFile();
    std::string getOutputFile();

    // parameter configuration
    void updateParameterFlags(uint8_t flags);
    void updateRiskAnalysis(int count);
    void updateComputeMode(ComputeMode mode);
    void updateMinReviewsPerSubmission(int count);
    void updateMaxReviewsPerReviewer(int count);
    
    // baldi para o risk_analysis:
    int skipReviewerId = -1;
    void setSubmissions (const std::vector<DataNode>&allNodes);
    void setReviewers(const std::vector<DataNode>& allNodes);
    int getTotalFlow();
    void generateVertices(); // TIREI DO PRIVATE
    void setSkipReviewerId(int id);

    private:
    /* This method is responsible for connecting vertex with edges in the graph, 
        how the connection is made depends on the parameter flag as follows:
        - PRIMARY_REVIEWER_EXPERTISE BIT ACTIVE (1) -> will include reveiwer primary expertise when connecting
        - SECONDARY_REVIEWER_EXPERTISE BIT ACTIVE (1) -> will include reviewer secondary expertise when connecting
        - PRIMARY_SUBMISSION_DOMAIN BIT ACTIVE (1) -> will include submission primary expertise when connecting
        - SECONDARY_SUBMISSION_DOMAIN BIT ACTIVE (1) -> will include submission secondary expertise when connecting */
    void buildGraphEdges(uint8_t flags);
    
    void cleanGraph();
    bool edgeExists(DataNode submission, DataNode reviewer);

    // file paths
    std::string inputFilePath;
    std::string outputFilePath;

    // parameters
    uint8_t parameterFlags; // only 4 LSB used
    ComputeMode computeMode;
    int riskAnalysis;
    int minReviewsPerSubmission;
    int maxReviewsPerReviewer;

    Graph<DataNode> graph;
    DataNode source;
    DataNode sink;
    std::vector<DataNode> submissions;
    std::vector<DataNode> reviewers;




};
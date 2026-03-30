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

/// @brief Enum to represent the compute mode for the solver, which determines the criteria used for matching submissions and reviewers
enum ComputeMode {
    NO_REPORT,
    PRIMARY_ONLY,
    SUBMISSION_SECONDARY,
    ALL
};

/// @brief Class to represent the solver for the assignment problem, which manages the graph representation of submissions and reviewers, parameters, and computation of assignments
/// @param graph The graph representation of submissions, reviewers, source, and sink
/// @param source The source vertex in the graph
/// @param sink The sink vertex in the graph
/// @param submissions A vector of DataNode objects representing the submissions
/// @param reviewers A vector of DataNode objects representing the reviewers
/// @param parameterFlags A bitmask representing the parameters for matching criteria
/// @param computeMode The compute mode for the solver, determining the criteria used for matching
/// @param riskAnalysis Flag indicating whether to perform risk analysis on the generated assignments
/// @param minReviewsPerSubmission The minimum number of reviews required per submission
/// @param maxReviewsPerReviewer The maximum number of reviews allowed per reviewer
/// @param inputFilePath The file path for the input data
/// @param outputFilePath The file path for the output results
/// @note The class provides methods for updating parameters, generating the graph, computing assignments using the Edmonds-Karp algorithm, and generating output files with the results
class Solver {
    public:
    Solver();

    /// @brief Generates the output file with the results of the assignment computation, including the matched pairs of submissions and reviewers, any missing reviews for submissions that did not meet the minimum requirements, and optionally a risk analysis of the generated assignments.
    /// @return The result of the output generation process, where 0 indicates success and -1 indicates an error (such as an invalid output file path or failure to open the file).
    int generateOutput();

    /// @brief Computes the assignment of reviewers to submissions based on the current graph, parameters, and compute mode. This method builds the edges of the graph according to the specified parameter flags, computes the maximum flow using the Edmonds-Karp algorithm, and returns the result of the computation.
    /// @return The result of the assignment computation, where 0 indicates success and any non-zero value indicates an error or failure in the computation process.
    int computeAssignment();

    /// @brief A method for testing and development purposes that populates the solver with hardcoded data for submissions and reviewers. This method can be used to quickly set up a test scenario without needing to parse input files, allowing for faster iteration during development.
    void processDummyInput(); // hard coded data for development

    // file handling
    /// @brief Updates the input file path for the solver, which is used to read the data for submissions and reviewers. This method allows dynamic configuration of the input source, enabling the solver to be used with different datasets without needing to modify the code.
    /// @param filePath The new file path for the input data
    void updateInputFile(std::string filePath);

    /// @brief Updates the output file path for the solver, which is used to write the results of the assignment computation. This method allows dynamic configuration of the output destination, enabling the solver to save results to different locations or files without needing to modify the code.
    /// @param filePath The new file path for the output results
    void updateOutputFile(std::string filePath);

    /// @brief Returns the current input file path for the solver.
    /// @return The file path for the input data
    std::string getInputFile();

    /// @brief Returns the current output file path for the solver.
    /// @return The file path for the output results
    std::string getOutputFile();

    // parameter configuration
    /// @brief Updates the parameter flags for the solver, which determine the criteria used for matching submissions and reviewers. The flags are represented as a bitmask, where each bit corresponds to a specific parameter (e.g., primary reviewer expertise, secondary submission domain). This method allows dynamic configuration of the matching criteria, enabling the solver to adapt to different requirements without needing to modify the code.
    /// @details The parameter flags are defined as follows:
    /// - PRIMARY_REVIEWER_EXPERTISE (bit 0): If set, the primary expertise of the reviewer will be considered when matching with submissions.
    /// - SECONDARY_REVIEWER_EXPERTISE (bit 1): If set, the secondary expertise of the reviewer will be considered when matching with submissions.
    /// - PRIMARY_SUBMISSION_DOMAIN (bit 2): If set, the primary domain of the submission will be considered when matching with reviewers.
    /// - SECONDARY_SUBMISSION_DOMAIN (bit 3): If set, the secondary domain of the submission will be considered when matching with reviewers.
    /// @param flags The new parameter flags as a bitmask
    void updateParameterFlags(uint8_t flags);

    /// @brief Updates the risk analysis setting for the solver, which determines whether to perform a risk analysis on the generated assignments. This method allows dynamic configuration of the risk analysis feature, enabling users to choose whether to include this analysis in the output results based on their needs.
    /// @param count The number of risk analyses to perform as configured in the project description (0 for no analysis, 1 for basic analysis, etc.)
    void updateRiskAnalysis(int count);

    /// @brief Updates the computation mode for the solver, which determines how the assignment computation is performed. This method allows dynamic configuration of the computation approach, enabling the solver to adapt to different scenarios without needing to modify the code.
    /// @param mode The new computation mode for the solver, represented as an enum value (e.g., NO_REPORT, PRIMARY_ONLY, SUBMISSION_SECONDARY, ALL)
    /// @details The computation modes are defined as follows:
    /// - NO_REPORT: The solver will compute the assignments without generating any report or output file
    /// - PRIMARY_ONLY: The solver will compute the assignments considering only the primary expertise and domain of reviewers and submissions
    /// - SUBMISSION_SECONDARY: The solver will compute the assignments considering the secondary domain of submissions in addition to the primary expertise and domain of reviewers and submissions
    /// - ALL: The solver will compute the assignments considering all criteria (primary and secondary expertise of reviewers, primary and secondary domain of submissions)
    void updateComputeMode(ComputeMode mode);

    /// @brief Updates the minimum number of reviews required per submission. This method allows dynamic configuration of the review requirements, enabling users to set different thresholds for the number of reviews needed for each submission without needing to modify the code.
    /// @param count The minimum number of reviews required per submission
    void updateMinReviewsPerSubmission(int count);

    /// @brief Updates the maximum number of reviews allowed per reviewer. This method allows dynamic configuration of the review limits, enabling users to set different thresholds for the number of reviews each reviewer can handle without needing to modify the code.
    /// @param count The maximum number of reviews allowed per reviewer
    void updateMaxReviewsPerReviewer(int count);

    // getters
    /// @brief Returns the current risk analysis setting for the solver.
    /// @return The number of risk analyses to perform as configured in the project description (0
    int getRiskAnalysis();

    /// @brief Returns the current minimum number of reviews required per submission.
    /// @return The minimum number of reviews required per submission
    int getMinReviewsPerSubmission();

    /// @brief Returns the current maximum number of reviewers allowed per submission.
    /// @return The maximum number of reviewers allowed per submission
    int getMaxReviewersPerSubmission();

    /// @brief Returns the current computation mode for the solver.
    /// @return The computation mode for the solver, represented as an enum value (e.g., NO_REPORT, PRIMARY_ONLY, SUBMISSION_SECONDARY, ALL)
    ComputeMode getComputeMode();

    /// @brief Returns the total number of submissions.
    /// @return The total number of submissions
    int getSubmissionAmount();

    /// @brief Returns the list of all submissions.
    /// @return A vector containing all submission nodes
    std::vector<DataNode> getSubmissions();

    /// @brief Returns the list of all reviewers.
    /// @return A vector containing all reviewer nodes
    std::vector<DataNode> getReviewers();
    
    // baldi para o risk_analysis:
    int skipReviewerId = -1;

    /// @brief Sets the list of all submissions.
    /// @param submissionNodes A vector containing the submission nodes to be set in the solver 
    void setSubmissions (const std::vector<DataNode>& submissionNodes);

    /// @brief Sets the list of all reviewers.
    /// @param reviewerNodes A vector containing the reviewer nodes to be set in the solver
    void setReviewers(const std::vector<DataNode>& reviewerNodes);

    /// @brief Returns the total flow in the graph. This method calculates the total flow from the source to the sink in the graph, which represents the number of successful matches between submissions and reviewers based on the current graph configuration and parameters.
    /// @return The total flow in the graph
    int getTotalFlow();

    /// @brief Generates the vertices in the graph for the source, sink, submissions, and reviewers. This method populates the graph with the appropriate vertices based on the current list of submissions and reviewers, as well as the source and sink nodes. It is typically called before building the edges of the graph to ensure that all necessary vertices are present for the assignment computation.
    void generateVertices(); // TIREI DO PRIVATE

    /// @brief Sets the ID of the reviewer to be skipped in the assignment process.
    /// @param id The ID of the reviewer to skip
    void setSkipReviewerId(int id);

    private:
    /* This method is responsible for connecting vertex with edges in the graph, 
        how the connection is made depends on the parameter flag as follows:
        - PRIMARY_REVIEWER_EXPERTISE BIT ACTIVE (1) -> will include reveiwer primary expertise when connecting
        - SECONDARY_REVIEWER_EXPERTISE BIT ACTIVE (1) -> will include reviewer secondary expertise when connecting
        - PRIMARY_SUBMISSION_DOMAIN BIT ACTIVE (1) -> will include submission primary expertise when connecting
        - SECONDARY_SUBMISSION_DOMAIN BIT ACTIVE (1) -> will include submission secondary expertise when connecting */
    /// @brief Builds the edges in the graph based on the specified parameter flags, connecting submissions to reviewers according to the matching criteria defined by the flags. This method iterates through all pairs of submissions and reviewers, checks the relevant attributes (primary/secondary expertise and domain) based on the active flags, and adds edges to the graph for valid matches. The resulting graph will be used for computing the assignments using the Edmonds-Karp algorithm.
    /// @param flags A bitmask representing the active parameter flags that determine the matching criteria for connecting submissions and reviewers
    /// @details The parameter flags are defined as follows:
    /// - PRIMARY_REVIEWER_EXPERTISE (bit 0): If set, the primary expertise of the reviewer will be considered when matching with submissions.
    /// - SECONDARY_REVIEWER_EXPERTISE (bit 1): If set, the secondary expertise of the reviewer will be considered when matching with submissions.
    /// - PRIMARY_SUBMISSION_DOMAIN (bit 2): If set, the primary domain of the submission will be considered when matching with reviewers.
    /// - SECONDARY_SUBMISSION_DOMAIN (bit 3): If set, the secondary domain of the submission will be considered when matching with reviewers.
    void buildGraphEdges(uint8_t flags);
    
    /// @brief Cleans the graph by removing all vertices and edges, effectively resetting the graph to an empty state. This method can be used to clear the graph before generating a new set of vertices and edges based on updated parameters or input data, ensuring that old data does not interfere with new computations.
    void cleanGraph();

    /// @brief Checks if an edge exists between a submission and a reviewer in the graph.
    /// @param submission The submission node
    /// @param reviewer The reviewer node
    /// @return True if the edge exists, false otherwise
    bool edgeExists(DataNode submission, DataNode reviewer);

    /// @brief Prints the current state of the graph, including the vertices and edges. This method is primarily used for debugging purposes to visualize the structure of the graph after generating vertices and edges, allowing developers to verify that the graph is constructed correctly according to the specified parameters and input data.
    void printGraph();

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
    DataNode source = {SOURCE, -1, -1, -1, "", "", ""};
    DataNode sink = {SINK, -2, -1, -1, "", "", ""};
    std::vector<DataNode> submissions;
    std::vector<DataNode> reviewers;




};
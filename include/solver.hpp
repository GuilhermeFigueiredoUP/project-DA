#pragma once

#include <string>

#define BIT(n) (1<<n)
// parameter flag values
#define PRIMARY_REVIEWER_EXPERTISE BIT(0)
#define SECONDARY_REVIEWER_EXPERTISE BIT(1)
#define PRIMARY_SUBMISSION_DOMAIN BIT(2)
#define SECONDARY_SUBMISSION_DOMAIN BIT(3)

// control flag values
#define GENERATE_ASSIGNMENTS BIT(0)
#define RISK_ANALYSIS BIT(1)

class Solver {
    public:
    Solver();

    int processInput();
    int generateOutput();
    int processDummyInput(); // hard coded data for development

    // file handling
    void updateInputFile(std::string filePath);
    void updateOutputFile(std::string filePath);
    std::string getInputFile();
    std::string getOutputFile();

    // parameter configuration
    void updateParameterFlags(uint8_t flags);
    void updateControlFlags(uint8_t flags);
    void updateMinReviewsPerSubmission(int count);
    void updateMaxReviewsPerReviewer(int count);

    private:
    // file paths
    std::string inputFilePath;
    std::string outputFilePath;

    // parameters
    uint8_t parameterFlags; // only 4 LSB used
    uint8_t controlFlags; // only 2 LSB used
    bool minReviewsPerSubmission;
    bool maxReviewsPerReviewer;

};
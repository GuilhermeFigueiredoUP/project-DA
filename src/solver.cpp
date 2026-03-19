#include "../include/solver.hpp"

Solver::Solver() {
    return;
}

int Solver::processInput() {
    // TODO: implement data file parsing
    return 1;
}

// utilises the values in the project description example for development purposes
int Solver::processDummyInput() {
    this->parameterFlags = 0x00 | PRIMARY_REVIEWER_EXPERTISE 
        | PRIMARY_SUBMISSION_DOMAIN | SECONDARY_SUBMISSION_DOMAIN;
    this->controlFlags = 0x00 | GENERATE_ASSIGNMENTS;
    this->maxReviewsPerReviewer = 4;
    this->minReviewsPerSubmission = 5;

    // TODO: implement submission and reviewer data
    return 1;
}

// --- File Handling ---
void Solver::updateInputFile(std::string filePath) {
    this->inputFilePath = filePath;
    return;
}

void Solver::updateOutputFile(std::string filePath) {
    this->outputFilePath = filePath;
    return;
}

std::string Solver::getInputFile() {
    return this->inputFilePath;
}

std::string Solver::getOutputFile() {
    return this->outputFilePath;
}

// --- Parameter Configuration ---
int Solver::generateOutput() {
    // TODO: implement output file generation
    return 1;
}

void Solver::updateParameterFlags(uint8_t flags) {
    this->parameterFlags = flags;
    return;
}

void Solver::updateControlFlags(uint8_t flags) {
    this-> controlFlags = flags;
    return;
}

void Solver::updateMinReviewsPerSubmission(int count) {
    this->minReviewsPerSubmission = count;
    return;
}

void Solver::updateMaxReviewsPerReviewer(int count) {
    this->maxReviewsPerReviewer = count;
    return;
}


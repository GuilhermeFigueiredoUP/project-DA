#include "../include/solver.hpp"

Solver::Solver() {
    return;
}

int Solver::processInput() {
    // TODO: implement data file parsing
    return 1;
}

// utilises the values in the project description example for development purposes
void Solver::processDummyInput() {
    this->parameterFlags = 0x00 | PRIMARY_REVIEWER_EXPERTISE 
        | PRIMARY_SUBMISSION_DOMAIN | SECONDARY_SUBMISSION_DOMAIN;
    this->controlFlags = 0x00 | GENERATE_ASSIGNMENTS;
    this->maxReviewsPerReviewer = 4;
    this->minReviewsPerSubmission = 5;

    // sink and source
    DataNode source = {SOURCE, NULL, NULL, NULL, NULL, NULL, NULL};
    this->graph.addVertex(source);
    DataNode sink = {SINK, NULL, NULL, NULL, NULL, NULL, NULL};
    this->graph.addVertex(sink);

    // submissions and reviewers
    DataNode data1 = {SUBMISSION, 31, 3, 4, "The Eternal Wheel of Reincarnation",
        "kholer@gmail.com", "Ralph Kohler"};
    this->graph.addVertex(data1);
    DataNode data2 = {SUBMISSION, 87, 1, -1, "GoDiva: A PIM Architecture",
        "draper@usc.edu", "Jeff Draper"};
    this->graph.addVertex(data2);

    DataNode data3 = {REVIEWER, 1, 2, -1, "Jaqueline N. Chame",
        "jchame@yahoo.com", ""};
    this->graph.addVertex(data3);
    DataNode data4 = {REVIEWER, 2, 1, 4, "Mary W. Hall",
        "mhall@hotmail.edu", ""};
    this->graph.addVertex(data4);

    // edge creation
    this->graph.addEdge(source, data1, this->minReviewsPerSubmission);
    this->graph.addEdge(source, data2, this->minReviewsPerSubmission);
    this->graph.addEdge(data3, sink, this->maxReviewsPerReviewer);
    this->graph.addEdge(data4, sink, this->maxReviewsPerReviewer);
    this->graph.addEdge(data1, data4, 1);
    this->graph.addEdge(data2, data4, 1);
    return;
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


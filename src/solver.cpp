#include "../include/solver.hpp"
#include "../include/edmonds_karp.hpp"
#include <cstdint>
Solver::Solver() {
    this->graph.addVertex(this->source);
    this->graph.addVertex(this->sink);
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
    this->riskAnalysis = 0;
    this->computeMode = PRIMARY_ONLY;
    this->maxReviewsPerReviewer = 4;
    this->minReviewsPerSubmission = 5;

    // submissions and reviewers
    DataNode data1 = {SUBMISSION, 31, 3, 4, "The Eternal Wheel of Reincarnation",
        "kholer@gmail.com", "Ralph Kohler"};
    DataNode data2 = {SUBMISSION, 87, 1, -1, "GoDiva: A PIM Architecture",
        "draper@usc.edu", "Jeff Draper"};

    DataNode data3 = {REVIEWER, 1, 2, -1, "Jaqueline N. Chame",
        "jchame@yahoo.com", ""};
    DataNode data4 = {REVIEWER, 2, 1, 4, "Mary W. Hall",
        "mhall@hotmail.edu", ""};

    this->submissions.push_back(data1);
    this->submissions.push_back(data2);
    this->reviewers.push_back(data3);
    this->reviewers.push_back(data4);
    return;
}

int Solver::computeAssignment() {
    buildGraphEdges(PRIMARY_REVIEWER_EXPERTISE | PRIMARY_SUBMISSION_DOMAIN);
    int ret = computeEdmondsKarp(this->graph, this->source, this->sink);
    if (ret != 0) return ret;
    switch(this->computeMode) {
        case ALL:
            buildGraphEdges(SECONDARY_REVIEWER_EXPERTISE | SECONDARY_SUBMISSION_DOMAIN | PRIMARY_REVIEWER_EXPERTISE | PRIMARY_SUBMISSION_DOMAIN);
            ret = computeEdmondsKarp(this->graph, this->source, this->sink);
            if (ret != 0) return ret;
            break;
        case SUBMISSION_SECONDARY:
            buildGraphEdges(PRIMARY_REVIEWER_EXPERTISE | PRIMARY_SUBMISSION_DOMAIN | SECONDARY_SUBMISSION_DOMAIN);
            ret = computeEdmondsKarp(this->graph, this->source, this->sink);
            if (ret != 0) return ret;
            break;
        default:
        break;
    }
    return 0;
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

void Solver::updateRiskAnalysis(int count) {
    this->riskAnalysis = count;
    return;
}

void Solver::updateComputeMode(ComputeMode mode) {
    this->computeMode = mode;
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

void Solver::cleanGraph() {
    for (DataNode &node : this->submissions) {
        this->graph.removeVertex(node);
    }
    for (DataNode &node : this->reviewers) {
        this->graph.removeVertex(node);
    }
    return;
}

void Solver::generateVertices() {
    cleanGraph();
    this->graph.addVertex(this->source);
    this->graph.addVertex(this->sink);
    for (DataNode &node : this->submissions) {
        this->graph.addVertex(node);
        this->graph.addEdge(this->source, node, this->minReviewsPerSubmission);
    }
    for (DataNode &node : this->reviewers) {
        if (node.id == this ->skipReviewerId) continue; 
        this->graph.addVertex(node);
        this->graph.addEdge(node, this->sink, maxReviewsPerReviewer);
    }
    return;
}

void Solver::buildGraphEdges(uint8_t flags) {
    for (DataNode &submissionNode : this->submissions) {
        for (DataNode &reviewerNode : this->reviewers) {
            
            if (reviewerNode.id == this -> skipReviewerId) continue; // usado para o riskanalysis VER
            if (edgeExists(submissionNode, reviewerNode)) continue;
            if ((flags & PRIMARY_SUBMISSION_DOMAIN) || ((flags & SECONDARY_SUBMISSION_DOMAIN) && submissionNode.secondaryDomain != -1)) {
                if (flags & PRIMARY_REVIEWER_EXPERTISE) this->graph.addEdge(submissionNode, reviewerNode, 1);
                else if ((flags & SECONDARY_REVIEWER_EXPERTISE) && (reviewerNode.secondaryDomain != -1)) this->graph.addEdge(submissionNode, reviewerNode, 1);
            }
        }
    }
}

bool Solver::edgeExists(DataNode submission, DataNode reviewer) {
    for (DataNode &submissionNode : this->submissions) {
        for (Edge<DataNode> *edge : this->graph.findVertex(submissionNode)->getAdj()) {
            if (edge->getDest()->getInfo() == reviewer) return true;
        }
    }
    return false;
}

// baldi
int Solver:: getTotalFlow(){
    double total=0;
    Vertex<DataNode>* sinkVert = graph.findVertex(this ->sink);
    if (sinkVert){
        for (auto e: sinkVert -> getIncoming()){
            total += e->getFlow();
        }
    }
    return (int) total;
}

void Solver:: setSubmissions (const std::vector<DataNode>& allNodes){
    this -> submissions.clear();
    for (const auto&n: allNodes){
        if (n.type == SUBMISSION) this -> submissions.push_back(n);
    }
}

// alteração baldi - os reviewers e as submissions estavam a ser misturados
void Solver :: setReviewers(const std::vector<DataNode>&allNodes){
     this -> submissions.clear();
    for (const auto&n: allNodes){
        if (n.type == REVIEWER) this -> reviewers.push_back(n);
    }
}

void Solver:: setSkipReviewerId(int id){
    this ->skipReviewerId=id;
}
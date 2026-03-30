/// @file solver.cpp
/// @brief Implements the Solver class, which is responsible for managing the graph structure, processing input data, computing the assignment of reviewers to submissions using the Edmonds-Karp algorithm, and generating the output file with the results. The Solver class includes methods for building the graph based on specified parameters, cleaning the graph, checking for edge existence, and printing the graph for debugging purposes. It also handles file paths for input and output data and provides a method for processing hardcoded dummy input for development testing.
/// @details The Solver class interacts with the Graph data structure to represent submissions, reviewers, source, and sink vertices, and uses the Edmonds-Karp algorithm to compute maximum flow for the assignment problem. The class also includes functionality for generating output files with the results of the assignment computation, including matched pairs and any missing reviews. The implementation is designed to be flexible, allowing for dynamic configuration of parameters and file paths, and includes methods for testing and debugging the graph structure. The code is written in C++17 and follows a modular design, separating concerns between graph management, flow computation, and output generation.

#include "../include/solver.hpp"
#include "../include/edmonds_karp.hpp"
#include "../include/risk_analysis.hpp"

#include <cstdint>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

Solver::Solver() {
    this->graph.addVertex(this->source);
    this->graph.addVertex(this->sink);
    return;
}

// utilises the values in the project description example, for development purposes
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
    this->generateVertices();
    std::cout << "[DEBUG] generated vertices" << std::endl;
    std::cout << "submission count: " << submissions.size() << std::endl;
    std::cout << "reveiewer count: " << reviewers.size() << std::endl;
    buildGraphEdges(PRIMARY_REVIEWER_EXPERTISE | PRIMARY_SUBMISSION_DOMAIN);
    std::cout << "[DEBUG] generated edges" << std::endl;
    printGraph(); // DEBUG purposes
    int ret = computeEdmondsKarp(this->graph, this->source, this->sink);
    std::cout << "[DEBUG] computed edmonds karp" << std::endl;
    if (ret != 0) return ret;
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

/**
 * @brief Generates the final formatted CSV output file from the Max-Flow graph results.
 * * This function iterates through the assignment graph, identifies successful matchings,
 * where Flow > 0, calculates missing reviews for unmet minimums, and writes the
 * formatted results to the specified output file.
 * * @param filename The name or path of the CSV file to be created.
 * @return int Returns 0 on success, or 1 if there was an error creating the file.
 */
int Solver::generateOutput() {
    //check output file path
    if (this->outputFilePath.empty()) {
        std::cout << "Error: Output file path is not set!\n";
        return -1;
    }

    std::ofstream outFile(this->outputFilePath);
    if (!outFile.is_open()) {
        std::cout << "Error: Failed to open output file: " << this->outputFilePath << "\n";
        return -1;
    }

    //structure to store the found matches
    struct MatchRecord {
        int subId;
        int revId;
        int domain;
    };
    std::vector<MatchRecord> matches;
    std::vector<DataNode> missingReviews;

    //traverse graph and extract assignments (flow > 0)
    for (DataNode &subNode : this->submissions) {
        Vertex<DataNode> *vertex = this->graph.findVertex(subNode);
        if (vertex == nullptr) continue;

        int currentReviewsAssigned = 0;

        //check edges leaving the submission
        for (auto edge : vertex->getAdj()) {
            DataNode destNode = edge->getDest()->getInfo();
            std::cout << "checking flow between " << subNode.id << " - "  << destNode.id << std::endl;

            //set the match
            if (edge->getFlow() > 0) {
                currentReviewsAssigned += edge->getFlow();
                std::cout << "found flow between: " << subNode.id << " - " << destNode.id << std::endl;

                //check domain that caused the match
                int matchedDomain = subNode.primaryDomain;
                if (subNode.primaryDomain == destNode.primaryDomain || subNode.primaryDomain == destNode.secondaryDomain) {
                    matchedDomain = subNode.primaryDomain;
                } else if (subNode.secondaryDomain != -1 && (subNode.secondaryDomain == destNode.primaryDomain || subNode.secondaryDomain == destNode.secondaryDomain)) {
                    matchedDomain = subNode.secondaryDomain;
                }

                matches.push_back({subNode.id, destNode.id, matchedDomain});
            }
        }

        //if submission didn't get enough reviewers, record it as missing
        if (currentReviewsAssigned < this->minReviewsPerSubmission) {
            missingReviews.push_back(subNode);
        }
    }

    //write results
    //1. submission matches
    std::sort(matches.begin(), matches.end(), [](MatchRecord &one, MatchRecord &two){
        if (one.subId == two.subId) return one.revId <= two.revId;
        else return one.subId <= two.subId;
    });
    outFile << "#SubmissionId,ReviewerId,Match\n";
    for (MatchRecord &match : matches) {
        outFile << match.subId << ", " << match.revId << ", " << match.domain << "\n";
    }

    //2. reviewer matches
    std::sort(matches.begin(), matches.end(), [](MatchRecord &one, MatchRecord &two){
        if (one.revId == two.revId) return one.subId <= two.subId;
        else return one.revId<= two.revId;
    });
    outFile << "#ReviewerId,SubmissionId,Match\n";
    for (MatchRecord &match : matches) {
        outFile << match.revId << ", " << match.subId << ", " << match.domain << "\n";
    }
    outFile << "#Total: " << matches.size() << "\n";
    
    //3. missing reviewers
    if (!missingReviews.empty()) {
        outFile << "#SubmissionId,Domain,MissingReviews\n";
        for (DataNode &subNode : missingReviews) {
            auto vertex = this->graph.findVertex(subNode);
            int currentReviewsAssigned = 0;
            for (auto edge : vertex->getAdj()) {
                if (edge->getDest()->getInfo().type == REVIEWER && edge->getFlow() > 0) {
                    currentReviewsAssigned += edge->getFlow();
                }
            }
            int missingCount = this->minReviewsPerSubmission - currentReviewsAssigned;
            outFile << subNode.id << ", " << subNode.primaryDomain << ", " << missingCount << "\n";
        }
    }
    //4. Risk Analysis
    if (this->riskAnalysis == 1 && missingReviews.empty()) {
        outFile << "#Risk Analysis: 1\n"; 
        std::vector<int> offenders = risk_analysis(this);
        if (!offenders.empty()) {
            outFile << offenders[0];
            for (int i = 1; i < offenders.size(); i++) {
                outFile << ", " << offenders[i];
            }
            outFile << "\n";
        }
    }
    outFile.close();
    return 0;
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
        std::cout << "adding submission with id: " << node.id << std::endl;
        this->graph.addVertex(node);
        this->graph.addEdge(this->source, node, this->minReviewsPerSubmission);
    }

    for (DataNode &node : this->reviewers) {
        if (node.id == this ->skipReviewerId) continue; 
        std::cout << "adding reviewer with id: " << node.id << std::endl;
        this->graph.addVertex(node);
        this->graph.addEdge(node, this->sink, maxReviewsPerReviewer);
    }
    return;
}

void Solver::buildGraphEdges(uint8_t flags) {
            std::cout << "PRIMARY SUBMISSION: " << (bool)(flags & PRIMARY_SUBMISSION_DOMAIN) << std::endl;
            std::cout << "PRIMARY REVIEWER: " << (bool)(flags & PRIMARY_REVIEWER_EXPERTISE) << std::endl;
            std::cout << "SECONDARY SUBMISSION: " << (bool)(flags & SECONDARY_SUBMISSION_DOMAIN) << std::endl;
            std::cout << "SECONDARY REVEIWER: " << (bool)(flags & SECONDARY_REVIEWER_EXPERTISE) << std::endl;
    for (DataNode &submissionNode : this->submissions) {
        for (DataNode &reviewerNode : this->reviewers) {
            std::cout << "analysing pair : " << submissionNode.id << " - " << submissionNode.primaryDomain << " " << submissionNode.secondaryDomain<< " <-> " << reviewerNode.id << " - " << reviewerNode.primaryDomain << " " << reviewerNode.secondaryDomain << " : ";
            
            if (reviewerNode.id == this -> skipReviewerId) {
                std::cout << "ignored (riskanalysis)" << std::endl;
                continue; // usado para o riskanalysis VER
            }
            if (edgeExists(submissionNode, reviewerNode)) {
                std::cout << "ignored (already exists)" << std::endl;
                continue;
            }
            if (submissionNode.primaryDomain == reviewerNode.primaryDomain) {
                std::cout << "match (primary -> primary)" << std::endl;
                this->graph.addEdge(submissionNode, reviewerNode, 1);
                continue;
            }
            if (SECONDARY_SUBMISSION_DOMAIN & flags) {
                if (submissionNode.secondaryDomain == reviewerNode.primaryDomain) {
                    std::cout << "match (secondary -> primary)" << std::endl;
                    this->graph.addEdge(submissionNode, reviewerNode, 1);
                    continue;
                }
            }
            if (SECONDARY_REVIEWER_EXPERTISE & flags) {
                if (submissionNode.primaryDomain == reviewerNode.secondaryDomain) {
                    std::cout << "match (primary -> secondary)" << std::endl;
                    this->graph.addEdge(submissionNode, reviewerNode, 1);
                    continue;
                }
            }
            if ((SECONDARY_REVIEWER_EXPERTISE & flags) && (SECONDARY_SUBMISSION_DOMAIN & flags)) {
                if (submissionNode.secondaryDomain == reviewerNode.secondaryDomain) {
                    std::cout << "match (secondary -> secondary)" << std::endl;
                    this->graph.addEdge(submissionNode, reviewerNode, 1);
                    continue;
                }
            }
            std::cout << "ignored (no match)" << std::endl;
        }
    }
}

bool Solver::edgeExists(DataNode submission, DataNode reviewer) {
    for (Edge<DataNode> *edge : this->graph.findVertex(submission)->getAdj()) {
        if (edge->getDest()->getInfo() == reviewer) return true;
    }
    return false;
}

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

void Solver:: setSubmissions (const std::vector<DataNode>& submissionNodes){
    this->submissions.clear();
    this->submissions = submissionNodes;
}

// alteração baldi - os reviewers e as submissions estavam a ser misturados
void Solver :: setReviewers(const std::vector<DataNode>& reviewerNodes){
    this->reviewers.clear();
    this->reviewers = reviewerNodes;
}

void Solver:: setSkipReviewerId(int id){
    this ->skipReviewerId=id;
}

void Solver::printGraph() {
    std::cout << "current Graph: " << std::endl;

    std::vector<Vertex<DataNode> *> submissionVertices;
    std::vector<Vertex<DataNode> *> reviewerVertices;
    Vertex<DataNode> *sourceVertex = this->graph.findVertex(this->source);
    for (Edge<DataNode> *edge : sourceVertex->getAdj()) {
        submissionVertices.push_back(edge->getDest());
        std::cout << "SOURCE -> sub: " << edge->getDest()->getInfo().id << std::endl;
    }
    for (Vertex<DataNode> *submissionVertex: submissionVertices) {
        for (Edge<DataNode> *edge : submissionVertex->getAdj()) {
            reviewerVertices.push_back(edge->getDest());
            std::cout << "sub: " << submissionVertex->getInfo().id << " -> rev: " << edge->getDest()->getInfo().id << std::endl;
        }
    }
    for (Vertex<DataNode> *reviewerVertex : reviewerVertices) {
        for (Edge<DataNode> *edge : reviewerVertex->getAdj()) {
            std::cout << "rev: " << reviewerVertex->getInfo().id << " -> ";
            if (edge->getDest()->getInfo() == this->sink) {
                std::cout << "SINK" << std::endl;
            } else std::cout << edge->getDest()->getInfo().id << std::endl;
        }
    }
    std::cout << "Graph finished!" << std::endl;
}

int Solver::getRiskAnalysis() {
    return this->riskAnalysis;
}

int Solver::getMinReviewsPerSubmission() {
    return this->minReviewsPerSubmission;
}

int Solver::getSubmissionAmount() {
    return this->submissions.size();
}

int Solver::getMaxReviewersPerSubmission() {
    return this->maxReviewsPerReviewer;
}

ComputeMode Solver::getComputeMode() {
    return this->computeMode;
}

std::vector<DataNode> Solver::getSubmissions() {
    return this->submissions;
}

std::vector<DataNode> Solver::getReviewers() {
    return this->reviewers;
}
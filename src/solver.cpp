#include "../include/solver.hpp"
#include "../include/edmonds_karp.hpp"
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
    this->generateVertices();
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
        auto vertex = this->graph.findVertex(subNode);
        if (!vertex) continue;

        int currentReviewsAssigned = 0;

        //check edges leaving the submission
        for (auto edge : vertex->getAdj()) {
            DataNode destNode = edge->getDest()->getInfo();

            //set the match
            if (destNode.type == REVIEWER && edge->getFlow() > 0) {
                currentReviewsAssigned += edge->getFlow();

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
    //1. missing reviewers
    if (!missingReviews.empty()) {
        outFile << "#SubmissionId, Domain, MissingReviews\n";
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
    //2. success
    else {
        std::sort(matches.begin(), matches.end(), [](const MatchRecord &a, const MatchRecord &b) {
            if (a.subId == b.subId) return a.revId < b.revId;
            return a.subId < b.subId;
        });

        outFile << "#SubmissionId, ReviewerId, Match\n";
        for (const auto &m : matches) {
            outFile << m.subId << ", " << m.revId << ", " << m.domain << "\n";
        }

        std::sort(matches.begin(), matches.end(), [](const MatchRecord &a, const MatchRecord &b) {
            if (a.revId == b.revId) return a.subId < b.subId;
            return a.revId < b.revId;
        });

        outFile << "#ReviewerId, SubmissionId, Match\n";
        for (const auto &m : matches) {
            outFile << m.revId << ", " << m.subId << ", " << m.domain << "\n";
        }

        outFile << "#Total: " << matches.size() << "\n";
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
        this->graph.addVertex(node);
        this->graph.addEdge(this->source, node, this->minReviewsPerSubmission);
    }
    for (DataNode &node : this->reviewers) {
        this->graph.addVertex(node);
        this->graph.addEdge(node, this->sink, maxReviewsPerReviewer);
    }
    return;
}

void Solver::buildGraphEdges(uint8_t flags) {
    for (DataNode &submissionNode : this->submissions) {
        for (DataNode &reviewerNode : this->reviewers) {
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
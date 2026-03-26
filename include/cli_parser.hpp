#pragma once
using namespace std;
#include "node_data.hpp"
#include <string>
#include <vector>

// parser for batch mode execution
int parseArguments(int argc, char *argv[], std:: vector<DataNode>& allNodes, Parameters& params, Control& ctrl) ;

// parser for menu mode execution
int parseInput(string inputFile, vector<DataNode>& allNodes,Parameters &params, Control&ctrl);


// --- helper methods ---
bool hasCsvExtension(const std::string &filename);

int submission_count(const std::vector<DataNode>& allNodes);
#pragma once
#include <vector>
#include <string>
using namespace std;
#include "node_data.hpp"


// parser for batch mode execution
int parseArguments(int argc, char *argv[]);

// parser for menu mode execution
int parseInput(string inputFile, vector<DataNode>& allNodes,Parameters &params, Control&ctrl);

// --- helper methods ---
bool hasCsvExtension(const std::string &filename);
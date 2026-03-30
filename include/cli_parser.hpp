#pragma once

#include "node_data.hpp"
#include "solver.hpp"

#include <string>
#include <vector>

// parser for batch mode execution
int parseArguments(int argc, char *argv[]);

// parser for menu mode execution
int parseInput(std::string inputFile, std::vector<DataNode>& allNodes,Parameters &params, Control&ctrl);


// --- helper methods ---
bool hasCsvExtension(const std::string &filename);

// --- terminal commands ---
int Terminal_cmd(std::string input, Solver& solver);
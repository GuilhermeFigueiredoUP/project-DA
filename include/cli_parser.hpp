
#pragma once

#include "node_data.hpp"
#include "solver.hpp"

#include <string>
#include <vector>

/// @brief analyses and validates the arguments from the command line
/// @param argc argument count
/// @param argv argument vector
/// @param allNodes reference to a vector to store the parsed data
/// @param params reference to a Parameters object
/// @param ctrl reference to a Control object
/// @return 0 on success and -1 if argument count is wrong/the flag is not "-b"/ files lack the ".csv" extension
int parseArguments(int argc, char *argv[], Solver &solver) ;

/// @brief parses a .csv file into data nodes(submissions and reviwers ), parameters and control settings
/// @param inputFile path to the input .csv file
/// @param allNodes reference to a vector where parsed DataNode objects will be stored
/// @param params reference to the Parameters object
/// @param ctrl reference to the Control object
/// @return 0 if parsing was sucessfull, -1 if an error occured
int parseInput(std::string inputFile, Solver &solver);

// --- helper methods ---
bool hasCsvExtension(const std::string &filename);

// --- terminal commands ---
int Terminal_cmd(std::string input, Solver& solver);

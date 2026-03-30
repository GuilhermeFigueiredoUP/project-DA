
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
/// @brief checks if a given filename has a .csv extension
/// @param filename the name of the file to check
/// @return true if the filename ends with .csv, false otherwise
bool hasCsvExtension(const std::string &filename);

// --- terminal commands ---
/**
 * @brief Processes and executes interactive terminal commands from the user.
 * * This function acts as the main parser for the interactive command-line interface (CLI).
 * It reads the raw string input provided by the user, validates the arguments, and
 * calls the corresponding methods on the Solver object to configure parameters,
 * load datasets, run the Max-Flow algorithm, or set output configurations.
 * * @param input The command string typed by the user in the terminal.
 * @param solver A reference to the main Solver instance that is modified or executed based on the command.
 * @return int Always returns 0 after processing the command.
 */
int Terminal_cmd(std::string input, Solver& solver);

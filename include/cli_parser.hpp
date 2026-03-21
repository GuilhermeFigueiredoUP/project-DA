#pragma once
#include "solver.hpp"

#include <string>

// parser for batch mode execution
int parseArguments(int argc, char *argv[]);

// parser for menu mode execution
int parseInput(std::string input, Solver& solver);

// --- helper methods ---
bool hasCsvExtension(const std::string &filename);
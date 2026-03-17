#pragma once

#include <string>

// parser for batch mode execution
int parseArguments(int argc, char *argv[]);

// parser for menu mode execution
int parseInput(std::string);

// --- helper methods ---
bool hasCsvExtension(const std::string &filename);
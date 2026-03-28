
#pragma once
using namespace std;
#include "node_data.hpp"
#include <string>
#include <vector>

/// @brief analyses and validates the arguments from the command line
/// @param argc argument count
/// @param argv argument vector
/// @param allNodes reference to a vector to store the parsed data
/// @param params reference to a Parameters object
/// @param ctrl reference to a Control object
/// @return 0 on success and -1 if argument count is wrong/the flag is not "-b"/ files lack the ".csv" extension
int parseArguments(int argc, char *argv[], std:: vector<DataNode>& allNodes, Parameters& params, Control& ctrl) ;

/// @brief parses a .csv file into data nodes(submissions and reviwers ), parameters and control settings
/// @param inputFile path to the input .csv file
/// @param allNodes reference to a vector where parsed DataNode objects will be stored
/// @param params reference to the Parameters object
/// @param ctrl reference to the Control object
/// @return 0 if parsing was sucessfull, -1 if an error occured
int parseInput(string inputFile, vector<DataNode>& allNodes,Parameters &params, Control&ctrl);






#include <string>
#include "../include/solver.hpp"
/// @brief determines which reviewers are essential to the system
/// For each reviewer, simulates the assignment process by temporarily removing them.
///If the required flow cannot be met without him, their ID is added to a list of offending reviewers
///The results are sorted and appended to the specified output file 
///@note the function returns immediately if RiskAnalysis is not set to 1
///@note this process uses a temporary Solver for each reviewer 
/// @param allNodes reference to the vector that contais the submissions/reviewers
/// @param params referenece to the Parameters object
/// @param ctrl reference to the Control object
/// @param outName name of the file where the risk analysis results will be appended
std::vector<int> risk_analysis (Solver *solver);
#include <vector>
#include "node_data.hpp"
/// @brief determines which reviewers are essential to the system
/// For each reviewer, simulates the assignment process by temporarily removing them.
///If the required flow cannot be met without him, their ID is added to a list of offending reviewers
///The results are sorted and appended to the specified output file 
///@note the function returns immediately if ctrl.RiskAnalysis is not in teh ]0,1] range
///@note this process uses a temporary Solver for each reviewer 
/// @param allNodes reference to the vector that contais the submissions/reviewers
/// @param params referenece to the Parameters object
/// @param ctrl reference to the Control object
/// @param outName name of the file where the risk analysis results will be appended
void risk_analysis (const std::vector<DataNode>& allNodes, const Parameters& params, const Control& ctrl, const std::string& outName);
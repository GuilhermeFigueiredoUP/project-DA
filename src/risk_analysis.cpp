/// @file risk_analysis.cpp
/// @brief Implements the risk analysis functionality for the solver, which identifies reviewers whose removal would lead to a decrease in the maximum flow.

#include "../include/risk_analysis.hpp"
#include <vector>
#include "../include/cli_parser.hpp" // de onde vem se tenho ou nao um nome para o meu ficheiro de output (caixas cinzentas da descricao do proj)
#include "../include/edmonds_karp.hpp"
#include "../data_structures/Graph.h"
#include <iostream>
#include <fstream>



std::vector<int> risk_analysis (Solver *solver){
    if (solver->getRiskAnalysis() != 1) return {};
    std::vector <int> offending_reviewers;

    int numero_submissions = solver->getSubmissionAmount();
    int requiredFlow = numero_submissions * solver->getMinReviewsPerSubmission();
    std::cout << "\n[DEBUG] Total Submissões: " << numero_submissions << std::endl;
    std::cout << "[DEBUG] Fluxo Necessário (Target): " << requiredFlow << std::endl;
    for (const auto& node : solver->getReviewers()){
        Solver tempSolver;
        // passamos os dados do parser para o solver
        tempSolver.setSubmissions( solver->getSubmissions());
        tempSolver.setReviewers(solver->getReviewers());

        tempSolver.updateMinReviewsPerSubmission(solver->getMinReviewsPerSubmission());
        tempSolver.updateMaxReviewsPerReviewer(solver->getMaxReviewersPerSubmission());

        tempSolver.updateComputeMode(solver->getComputeMode());

        tempSolver.setSkipReviewerId(node.id); // solver,ignora este node 

        tempSolver.generateVertices();
        tempSolver.computeAssignment();

        int finalFlow = tempSolver.getTotalFlow();
        std::cout << "[DEBUG] A testar sem o Revisor " << node.id << " | Fluxo obtido: " << finalFlow << std::endl;
        if (finalFlow< requiredFlow) offending_reviewers.push_back(node.id);

    }

// escrita no output file: pode estar estipulado no parser ou nao

    std::sort(offending_reviewers.begin(), offending_reviewers.end());

    return offending_reviewers;
}
#include <vector>
#include "cli_parser.hpp" // de onde vem se tenho ou nao um nome para o meu ficheiro de output (caixas cinzentas da descricao do proj)
#include "edmonds_karp.hpp"
#include "Graph.h"
#include "solver.hpp"
#include <iostream>
#include<fstream>


// o sub e o gajo que eu vou tirar para ver se ele da na mm c o minreviewers p submission: RIGHT?
void risk_analysis (const std::vector<DataNode>& allNodes, const Parameters& params, const Control& ctrl, const std::string& outName){
    if (ctrl.RiskAnalysis <=0||ctrl.RiskAnalysis >1) return ;
    std::vector <int> offending_reviewers;

    int numero_submissions = 0;
    for (const auto& node: allNodes){
        if (node.type == SUBMISSION){
            numero_submissions++;
        }
    }
    int requiredFlow = numero_submissions * params.MinReviewsPerSubmission;
    std::cout << "\n[DEBUG] Total Submissões: " << numero_submissions << std::endl;
    std::cout << "[DEBUG] Fluxo Necessário (Target): " << requiredFlow << std::endl;
    for (const auto& node : allNodes){
        if (node.type == REVIEWER){
            Solver tempSolver;
            // passamos os dados do parser para o solver
            tempSolver.setSubmissions( allNodes);
            tempSolver.setReviewers(allNodes); // filtra oq sao reviews

            tempSolver.updateMinReviewsPerSubmission(params.MinReviewsPerSubmission);
            tempSolver.updateMaxReviewsPerReviewer(params.MaxReviewsPerReviewer);

            if (ctrl.GenerateAssignments ==1) tempSolver.updateComputeMode(PRIMARY_ONLY);
            if (ctrl.GenerateAssignments ==2) tempSolver.updateComputeMode(SUBMISSION_SECONDARY);
            if (ctrl.GenerateAssignments ==3) tempSolver.updateComputeMode(ALL);

            tempSolver.setSkipReviewerId(node.id); // solver,ignora este node 

            tempSolver.generateVertices();
            tempSolver.computeAssignment();

            int finalFlow = tempSolver.getTotalFlow();
            std::cout << "[DEBUG] A testar sem o Revisor " << node.id << " | Fluxo obtido: " << finalFlow << std::endl;
            if (finalFlow< requiredFlow) offending_reviewers.push_back(node.id);


        }
    }

// escrita no output file: pode estar estipulado no parser ou nao

    std::sort(offending_reviewers.begin(), offending_reviewers.end());
    
    std::ofstream outFile(outName,std::ios:: app); // append ao outputfile
    if (outFile.is_open()){
        outFile << "#Risk Analysis: " << ctrl.RiskAnalysis<< "\n";
        for (size_t i=0; i< offending_reviewers.size();i++){
            outFile << offending_reviewers[i]<< (i == offending_reviewers.size()-1? "" : ", ");
        }
        outFile <<"\n";
        outFile.close();
    }

}
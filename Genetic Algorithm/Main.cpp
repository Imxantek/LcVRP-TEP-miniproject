#include <iostream>
#include <vector>
#include <string>
#include <ctime>   // Do pomiaru czasu
#include "ProblemLoader.h"
#include "ProblemData.h"
#include "CEvaluator.h"
#include "CGeneticAlgorithm.h"

using namespace std;
void StartOptimization(const string& folder_name, const string& instance_name, int num_groups, int max_iterations) {
	ProblemLoader problem_loader(folder_name, instance_name);
	ProblemData problem_data = problem_loader.LoadProblem();

	CEvaluator evaluator(problem_data, num_groups);
	CGeneticAlgorithm optimizer(evaluator, 200, 0.75, 0.2);

	optimizer.initialize();
	for (int i = 0; i < max_iterations; i++) {
		optimizer.runIteration();
	}

	std::vector<int>* bestSolution = optimizer.GetCurrentBest();
	cout << "Best Fitness: " << evaluator.Evaluate(*bestSolution);
}
int main() {
	int num_groups = 5;
	int max_iterations = 20000;
	StartOptimization("Vrp-Set-A", "A-n32-k5", num_groups, max_iterations);
}
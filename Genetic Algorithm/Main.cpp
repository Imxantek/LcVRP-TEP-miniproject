#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include "ProblemLoader.h"
#include "ProblemData.h"
#include "CEvaluator.h"
#include "CGeneticAlgorithm.h"

using namespace std;
void StartOptimizationByNums(const string& folder_name, const string& instance_name, int num_groups, int max_iterations) {
	ProblemLoader problem_loader(folder_name, instance_name);
	ProblemData problem_data = problem_loader.Load();
	std::mt19937 re;
	CEvaluator evaluator(problem_data, num_groups);
	CGeneticAlgorithm optimizer(evaluator, 200, 0.70, 0.2, re);

	optimizer.initialize();
	for (int i = 0; i < max_iterations; i++) {
		optimizer.runIteration();
	}

	std::vector<int>* bestSolution = optimizer.GetCurrentBest();
	cout << "Best Fitness: " << evaluator.Evaluate(*bestSolution).first;
}
void StartOptimizationByTime(const string& folder_name, const string& instance_name, int num_groups, int maxTime) {
	ProblemLoader problem_loader(folder_name, instance_name);
	ProblemData problem_data = problem_loader.Load();
	std::mt19937 re;
	CEvaluator evaluator(problem_data, num_groups);
	CGeneticAlgorithm optimizer(evaluator, 2048, 0.75, 0.05, re);
	optimizer.initialize();
	clock_t start_time = clock();
	int iteration_no = 0;
	while (true) {
		optimizer.runIteration();
		clock_t current_time = clock();
		double elapsed_time = double(current_time - start_time) / CLOCKS_PER_SEC;
		if (elapsed_time >= maxTime) {
			break;
		}
		std::cout << "Elapsed Time: " << elapsed_time << " seconds\n";
		std::cout << "best fitness: " << optimizer.getBestFitness() << "\n";
		std::cout << "iteration no: " << iteration_no++ << "\n";

	}
	std::vector<int>* bestSolution = optimizer.GetCurrentBest();
	cout << "Best Fitness: " << evaluator.Evaluate(*bestSolution).first;
}
int main() {
	int time_groups = 131;
	int maxTime = 10000000; // seconds
	StartOptimizationByTime("Vrp-Set-X", "X-n655-k131", time_groups, maxTime);
	int num_groups = 5;
	int max_iterations = 40000;
	//StartOptimizationByNums("Vrp-Set-A", "A-n32-k5", num_groups, max_iterations);
}
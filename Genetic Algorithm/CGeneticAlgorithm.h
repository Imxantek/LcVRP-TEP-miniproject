#pragma once
#include<vector>
#include<utility>
#include"CIndividual.h"
#include"CEvaluator.h"
#ifndef CGENETICALGORITHM_H_
#define CGENETICALGORITHM_H_
class CGeneticAlgorithm {
private:
	CEvaluator& evaluator;
	std::vector<int> current_best;
	std::vector<CIndividual> population;
	std::mt19937& re;
	
	
	int popSize;
	double crossProb;
	double mutProb;



	double bestFitness=INT_MAX;
	int lowerBound = 0;
	int upperBound = evaluator.GetNumGroups()-1;
	int selection();
public:
	CGeneticAlgorithm(CEvaluator& evaluator, int popSize, double crossProb, double mutProb, std::mt19937& re) : evaluator(evaluator),
		popSize(popSize), crossProb(crossProb), mutProb(mutProb), re(re) {};
	void initialize();
	void runIteration();
	std::vector<int>* GetCurrentBest();
	double getBestFitness() { return bestFitness; }
};
#endif CGENETICALGORITHM_H_
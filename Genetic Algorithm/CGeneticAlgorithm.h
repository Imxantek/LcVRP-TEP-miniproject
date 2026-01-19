#pragma once
#include<vector>
#include"CIndividual.h"
#include"CEvaluator.h"
#ifndef CGENETICALGORITHM_H_
#define CGENETICALGORITHM_H_
class CGeneticAlgorithm {
private:
	int popSize;
	double crossProb;
	double mutProb;
	std::vector<CIndividual*> population;
	CEvaluator* evaluator;
	void selection();
public:
	CGeneticAlgorithm();
	~CGeneticAlgorithm();
	CGeneticAlgorithm(const CGeneticAlgorithm& pcOther);
	void initialize();
	void runIteration();
	void run();
	void getBestSolution();
};
#endif CGENETICALGORITHM_H_
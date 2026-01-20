#include "CGeneticAlgorithm.h"
std::vector<int>* CGeneticAlgorithm::GetCurrentBest() {
	return &current_best;
}
void CGeneticAlgorithm::initialize() {
	population.reserve(popSize);
	bestFitness = INT_MAX;
	int solutionSize = evaluator.GetNumCustomers();
	for (int i = 0; i < popSize; i++) {
		CIndividual ind(lowerBound, upperBound, solutionSize, re);
		double fitness = ind.calculateFitness(evaluator);
		population.push_back(ind); //optymalne, z racji na semantyke przenoszenia -> vector jest przenoszalny RVO siê tym zajmie
		if (fitness < bestFitness) {
			current_best = ind.getGenotype();
		}
	}
}
void CGeneticAlgorithm::runIteration() {
	std::vector<CIndividual> newPopulation;
	while (newPopulation.size() < popSize) {
		const CIndividual &parentA = population[selection()];
		const CIndividual &parentB = population[selection()];
		std::pair<CIndividual, CIndividual> cross = parentA.cross(parentB, crossProb, re);
		newPopulation.push_back(cross.first);
		newPopulation.push_back(cross.second);
	}
	for (CIndividual &i : newPopulation) {
		i.mutate(mutProb, re, lowerBound, upperBound);
		if (i.getChanged()) {
			i.calculateFitness(evaluator);
			i.setChanged(false);
		}
		if (i.getFitness() < bestFitness) {
			current_best = i.getGenotype();
		}
	}
	population = newPopulation;
}

int CGeneticAlgorithm::selection() {
	std::uniform_int_distribution<int> randInt(0, popSize-1);
	int r1 = randInt(re), r2 = randInt(re);
	if (population[r1].getFitness() > population[r2].getFitness()) {
		return r2;
	}
	return r1;
}
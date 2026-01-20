#include "CGeneticAlgorithm.h"
std::vector<int>* CGeneticAlgorithm::GetCurrentBest() {
	return &current_best;
}
void CGeneticAlgorithm::initialize() {
	population.resize(popSize);
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
		CIndividual parentA = population[selection()];
		CIndividual parentB = population[selection()];
		newPopulation.push_back(parentA.cross(parentB, crossProb, re).first);
		newPopulation.push_back(parentB.cross(parentA, crossProb, re).second);
	}
	for (CIndividual i : newPopulation) {
		i.mutate(mutProb, re);
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
	std::uniform_int_distribution<int> randInt(0, popSize);
	int r1 = randInt(re), r2 = randInt(re);
	if (population[r1].getFitness() > population[r2].getFitness()) {
		return r2;
	}
	return r1;
}
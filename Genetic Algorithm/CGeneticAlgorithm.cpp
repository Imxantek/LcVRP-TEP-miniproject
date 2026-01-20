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
		ind.calculateFitness(evaluator);
		
		population.push_back(ind); //optymalne, z racji na semantyke przenoszenia -> vector jest przenoszalny RVO siê tym zajmie
		if (ind.getFitness() < bestFitness) {
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
		if (newPopulation.size() == popSize) { break; }
		newPopulation.push_back(cross.second);
	}
	CIndividual elite;
	elite.setGenotype(current_best);
	elite.calculateFitness(evaluator);
	double worstFitness=-1.0;
	int worstIndex=0;
	for (int i = 0; i < newPopulation.size(); i++) {
		newPopulation[i].mutate(mutProb, re, lowerBound, upperBound);
		
		newPopulation[i].calculateFitness(evaluator);
		
		double fit = newPopulation[i].getFitness(); 
		if (fit < bestFitness) {
			bestFitness = fit;
			current_best = newPopulation[i].getGenotype();
		}

		// Szukamy najs³abszego ogniwa w nowym pokoleniu
		if (i == 0 || fit > worstFitness) {
			worstFitness = fit;
			worstIndex = i;
		}
	}
	if (bestFitness < worstFitness) {
		CIndividual elite;
		elite.setGenotype(current_best);
		elite.calculateFitness(evaluator); // Wa¿ne: policz fitness elity!
		newPopulation[worstIndex] = elite;
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
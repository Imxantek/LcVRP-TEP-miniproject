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
    newPopulation.reserve(popSize);

    // 1. Elityzm (bez zmian)
    CIndividual elite;
    elite.setGenotype(current_best);
    elite.calculateFitness(evaluator);

    // BONUS: Spróbujmy jeszcze ulepszyæ elitê Local Searchem!
    elite.localSearch(evaluator, re);
    if (elite.getFitness() < bestFitness) {
        bestFitness = elite.getFitness();
        current_best = elite.getGenotype();
    }

    newPopulation.push_back(std::move(elite));

    while (newPopulation.size() < popSize) {
        const CIndividual& parentA = population[selection()];
        const CIndividual& parentB = population[selection()];

        std::pair<CIndividual, CIndividual> children = parentA.cross(parentB, crossProb, re);

        // Dziecko 1
        children.first.mutate(mutProb, re, lowerBound, upperBound);
        // TUTAJ ZMIANA: Lekkie douczanie ka¿dego dziecka
        // Mo¿esz to robiæ z pewnym prawdopodobieñstwem, np. 10%, ¿eby nie zabiæ wydajnoœci
        children.first.localSearch(evaluator, re);
        children.first.calculateFitness(evaluator);

        if (children.first.getFitness() < bestFitness) {
            bestFitness = children.first.getFitness();
            current_best = children.first.getGenotype();
        }
        newPopulation.push_back(std::move(children.first));

        // Dziecko 2 (analogicznie)
        if (newPopulation.size() < popSize) {
            children.second.mutate(mutProb, re, lowerBound, upperBound);
            children.second.localSearch(evaluator, re); // Douczanie
            children.second.calculateFitness(evaluator);

            if (children.second.getFitness() < bestFitness) {
                bestFitness = children.second.getFitness();
                current_best = children.second.getGenotype();
            }
            newPopulation.push_back(std::move(children.second));
        }
    }
    population = std::move(newPopulation);
}
int CGeneticAlgorithm::selection() {
	std::uniform_int_distribution<int> randInt(0, popSize-1);
	int r1 = randInt(re), r2 = randInt(re);
	if (population[r1].getFitness() > population[r2].getFitness()) {
		return r2;
	}
	return r1;
}
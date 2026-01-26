#include "CGeneticAlgorithm.h"
#include <windows.h>
CGeneticAlgorithm::CGeneticAlgorithm(CEvaluator& evaluator, int popSize, double crossProb, double mutProb, std::mt19937& re)
    : evaluator(evaluator), popSize(popSize), crossProb(crossProb), mutProb(mutProb), re(re)
{
    this->lowerBound = 0;
    this->upperBound = evaluator.GetNumGroups(); 
    this->bestFitness = DBL_MAX;
    this->itCounter = 0;
}

std::vector<int>* CGeneticAlgorithm::GetCurrentBest() {
    return &current_best;
}
void CGeneticAlgorithm::initialize() {
    population.reserve(popSize);
    bestFitness = DBL_MAX;
    int solutionSize = evaluator.GetNumCustomers();
    for (int i = 0; i < popSize; i++) {
        CIndividual ind(lowerBound, upperBound, solutionSize, re);
        population.push_back(ind); //optymalne, z racji na semantyke przenoszenia -> vector jest przenoszalny RVO siê tym zajmie
    }
}
void CGeneticAlgorithm::runIteration() {
    // 1. Ocena populacji i szukanie najlepszego (to jest szybkie)
    for (CIndividual& ind : population) {
        ind.calculateFitness(evaluator);
        if (ind.getFitness() < bestFitness) {
            bestFitness = ind.getFitness();
            current_best = ind.getGenotype();
            itCounter++;
        }
    }

    // Zabezpieczenie na start
    if (current_best.empty() && !population.empty()) {
        current_best = population[0].getGenotype();
        bestFitness = population[0].getFitness();
    }

    std::vector<CIndividual> newPopulation;
    newPopulation.reserve(popSize);



    std::sort(population.begin(), population.end(),
        [](CIndividual& a, CIndividual& b) {
            return a.getFitness() < b.getFitness();
        });

    int eliteSize = 5;
    for (int i = 0; i < eliteSize; i++) {

        newPopulation.push_back(population[i]);
        if (i == 0) {
            newPopulation.back().localSearch(evaluator, re); // opcjonalne
            newPopulation.back().calculateFitness(evaluator); // jeśli localSearch był
        }
    }

    while (newPopulation.size() < popSize) {
        const CIndividual& parentA = population[selection()];
        const CIndividual& parentB = population[selection()];

        std::pair<CIndividual, CIndividual> children = parentA.cross(parentB, crossProb, re);

        children.first.mutate(mutProb, re, lowerBound, upperBound);
        children.second.mutate(mutProb, re, lowerBound, upperBound);


        newPopulation.push_back(std::move(children.first));
        if (newPopulation.size() < popSize) {
            newPopulation.push_back(std::move(children.second));
        }
    }

    population = std::move(newPopulation);
    itCounter++;
    printValidity(population);
    
}
int CGeneticAlgorithm::selection() {
    std::uniform_int_distribution<int> randInt(0, popSize - 1);
    int bestIdx = randInt(re);
    double bestFit = population[bestIdx].getFitness();

    // Turniej 10% populacji (lub np. stałe 10-20 osobników)
    int tournamentSize = popSize * 0.1;

    for (int i = 0; i < tournamentSize; i++) {
        int challengerIdx = randInt(re);
        double challengerFit = population[challengerIdx].getFitness();
        if (challengerFit < bestFit) { // Szukamy minimum
            bestFit = challengerFit;
            bestIdx = challengerIdx;
        }
    }
    return bestIdx;
}

void CGeneticAlgorithm::printValidity(std::vector<CIndividual>& population) {
	int validCount = 0;
    for(auto& individual : population) {
		if(individual.isValid()) {
            validCount++;
		}
	}
	std::cout << validCount << " out of " << population.size() << " individuals are valid." << std::endl;
}

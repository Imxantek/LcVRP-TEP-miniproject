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
        population.push_back(ind); //optymalne, z racji na semantyke przenoszenia -> vector jest przenoszalny RVO sie tym zajmie
    }
}

void CGeneticAlgorithm::runIteration() {
    for (CIndividual& ind : population) {
        ind.calculateFitness(evaluator);
        if (ind.getFitness() < bestFitness) {
            bestFitness = ind.getFitness();
            current_best = ind.getGenotype();
            itCounter = 0; 
        }
    }
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
    bool isStagnating = (itCounter > 200);
    bool isCataclysm = (itCounter > 600);
    int eliteSize = isStagnating ? 1 : (popSize * 0.30);

    for (int i = 0; i < eliteSize; i++) {
        newPopulation.push_back(population[i]);
        if (i == 0 && !isStagnating) {
            newPopulation.back().localSearch(evaluator, re);
            newPopulation.back().calculateFitness(evaluator);
        }
    }

    while (newPopulation.size() < popSize) {
        const CIndividual& parentA = population[selection(isStagnating)];
        const CIndividual& parentB = population[selection(isStagnating)];

        std::pair<CIndividual, CIndividual> children = parentA.cross(parentB, crossProb, re);
        if (isCataclysm) {
            int solutionSize = evaluator.GetNumCustomers();
            while (newPopulation.size() < popSize) {
                CIndividual randomInd(lowerBound, upperBound, solutionSize, re);
                newPopulation.push_back(std::move(randomInd));
            }
            itCounter = 100;
        }
        if (isStagnating) {
            children.first.aggresiveMutate(0.2, re, lowerBound, upperBound);
            children.second.aggresiveMutate(0.2, re, lowerBound, upperBound);
        }
        else {
            children.first.mutate(mutProb, re, lowerBound, upperBound);
            children.second.mutate(mutProb, re, lowerBound, upperBound);
        }
        newPopulation.push_back(std::move(children.first));
        if (newPopulation.size() < popSize) {
            newPopulation.push_back(std::move(children.second));
        }
    }
    if (!isCataclysm) {
        itCounter++;
    }
    population = std::move(newPopulation);
    printValidity(population); 
    std::cout << "current itCounter: " << itCounter << std::endl;
}


int CGeneticAlgorithm::selection(bool isStagnating) {
    int range = isStagnating ? popSize : (popSize * 0.4); 
    if (range > popSize) range = popSize;
    if (range < 1) range = 1;
    std::uniform_int_distribution<int> randInt(0, range - 1);
    int bestIdx = randInt(re);
    double bestFit = population[bestIdx].getFitness();
    int tournamentSize = 2; 
    for (int i = 0; i < tournamentSize; i++) {
        int challengerIdx = randInt(re);
        double challengerFit = population[challengerIdx].getFitness();
        if (challengerFit < bestFit) {
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

#include "CGeneticAlgorithm.h"
#include<windows.h>
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
    for (CIndividual& ind : population) {
        ind.calculateFitness(evaluator);
        if (ind.getFitness() < bestFitness) {
            bestFitness = ind.getFitness();
            current_best = ind.getGenotype();
            itCounter++;
        }
    }

    if (current_best.empty() && !population.empty()) {
        current_best = population[0].getGenotype();
        bestFitness = population[0].getFitness();
    }

    std::vector<CIndividual> newPopulation;
    newPopulation.reserve(popSize);

    // Tworzenie elity
    CIndividual elite;
    elite.setGenotype(current_best); // Teraz current_best jest bezpieczny (bez -1)
    elite.calculateFitness(evaluator); // To teraz zadziała poprawnie

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
        if (itCounter < 1000) {
            children.first.mutate(mutProb, re, lowerBound, upperBound);
        }
        else {
            children.first.mutate(mutProb * 2, re, lowerBound, upperBound);
        }
        children.first.localSearch(evaluator, re);
        children.first.calculateFitness(evaluator);

        if (children.first.getFitness() < bestFitness) {
            bestFitness = children.first.getFitness();
            current_best = children.first.getGenotype();
        }
        newPopulation.push_back(std::move(children.first));
        if (newPopulation.size() < popSize) {
            if (itCounter < 1000) {
                children.second.mutate(mutProb, re, lowerBound, upperBound);
            }
            else {
                children.second.mutate(mutProb * 2, re, lowerBound, upperBound);
            }
            children.second.localSearch(evaluator, re);
            children.second.calculateFitness(evaluator);

            if (children.second.getFitness() < bestFitness) {
                bestFitness = children.second.getFitness();
                current_best = children.second.getGenotype();
            }
            newPopulation.push_back(std::move(children.second));
        }
    }
    population = std::move(newPopulation);
    itCounter++;
	//printValidity(population);
    //Sleep(500);
}
int CGeneticAlgorithm::selection() {
    std::uniform_int_distribution<int> randInt(0, popSize - 1);
    int r1 = randInt(re), r2 = randInt(re);
    if (population[r1].getFitness() > population[r2].getFitness()) {
        return r2;
    }
    return r1;
}

void CGeneticAlgorithm::printValidity(std::vector<CIndividual>& population) {
    for(auto& individual : population) {
		std::cout << individual.isValid() << " ";
	}
	std::cout << std::endl; 
}

#include "CIndividual.h"
CIndividual::CIndividual(int lowerBound, int upperBound, int solutionSize, std::mt19937& re) {
    this->fitnessVal = DBL_MAX; 
    this->valid = false;
    std::uniform_int_distribution<int> dist(lowerBound, upperBound-1);
    for (int i = 0; i < solutionSize; i++) {
        genotype.push_back(dist(re));
    }
}
double CIndividual::calculateFitness(const CEvaluator& evaluator) {
    std::pair<double, bool> result = evaluator.Evaluate(this->genotype);
    this->fitnessVal = result.first;
    this->valid = result.second;
    return fitnessVal;
    
}
std::pair<CIndividual, CIndividual> CIndividual::cross(const CIndividual& pcOther, double crossProb, std::mt19937& re) const {
    std::uniform_real_distribution<double> d_dist(0, 1);
    if (d_dist(re) > crossProb) { // Jeśli nie krzyżujemy, zwracamy kopie
        return std::make_pair(*this, pcOther);
    }

    CIndividual child1 = *this;
    CIndividual child2 = pcOther;
    std::uniform_int_distribution<int> cutDist(1, genotype.size() - 2);
    int cutPoint = cutDist(re);

    for (int i = cutPoint; i < genotype.size(); i++) {
        child1.genotype[i] = pcOther.genotype[i];
        child2.genotype[i] = this->genotype[i];
    }

    return std::make_pair(child1, child2);
}

void CIndividual::localSearch(const CEvaluator& evaluator, std::mt19937& re) {
    // Zwiększamy liczbę prób - sprawdzamy 20 klientów zamiast 1
    int clientsToCheck = 20;

    std::uniform_int_distribution<int> distGen(0, genotype.size() - 1);
    std::uniform_int_distribution<int> distGroup(0, evaluator.GetNumGroups() - 1);

    for (int k = 0; k < clientsToCheck; k++) {
        int customerIdx = distGen(re);
        int originalGroup = genotype[customerIdx];

        // Pobieramy aktualny fitness (możesz użyć this->fitnessVal, jeśli jest aktualny)
        double currentFitness = fitnessVal;

        // Próbujemy 10 razy znaleźć lepszą grupę dla tego klienta
        for (int i = 0; i < 10; i++) {
            int newGroup = distGroup(re);
            if (newGroup == originalGroup) continue;

            genotype[customerIdx] = newGroup;

            // Szybka ocena zmiany
            std::pair<double, bool> result = evaluator.Evaluate(genotype);
            double newFitness = result.first;

            if (newFitness < currentFitness) {
                // Znaleziono poprawę! Zostawiamy zmianę.
                fitnessVal = newFitness;
                valid = result.second; // Aktualizujemy flagę valid
                currentFitness = newFitness;
                originalGroup = newGroup;
                isChanged = true;
                break; // Idziemy do następnego klienta
            }
            else {
                genotype[customerIdx] = originalGroup; // Cofamy zmianę
            }
        }
    }
}
void CIndividual::mutate(double mutProb, std::mt19937& re, int lowerBound, int upperBound) {
    std::uniform_real_distribution<double> d_dist(0, 1);
    std::uniform_int_distribution<int> int_dist(lowerBound, upperBound-1);
    for (int& gene : genotype) {
        if (mutProb > d_dist(re)) {
            isChanged = true;
            gene = int_dist(re);
        }
    }
}

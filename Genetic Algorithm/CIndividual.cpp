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
    if (d_dist(re) > crossProb) { 
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
    int clientsToCheck = 20;
    std::uniform_int_distribution<int> distGen(0, genotype.size() - 1);
    std::uniform_int_distribution<int> distGroup(0, evaluator.GetNumGroups() - 1);
    for (int k = 0; k < clientsToCheck; k++) {
        int customerIdx = distGen(re);
        int originalGroup = genotype[customerIdx];
        double currentFitness = fitnessVal;
        for (int i = 0; i < 10; i++) {
            int newGroup = distGroup(re);
            if (newGroup == originalGroup) continue;
            genotype[customerIdx] = newGroup;
            std::pair<double, bool> result = evaluator.Evaluate(genotype);
            double newFitness = result.first;
            if (newFitness < currentFitness) {
                fitnessVal = newFitness;
                valid = result.second; 
                currentFitness = newFitness;
                originalGroup = newGroup;
                isChanged = true;
                break; 
            }
            else {
                genotype[customerIdx] = originalGroup; 
            }
        }
    }
}
void CIndividual::mutate(double mutProb, std::mt19937& re, int lowerBound, int upperBound) {
    std::uniform_real_distribution<double> d_dist(0, 1);
    for (int i = 0; i < genotype.size(); i++) {
        if (d_dist(re) < mutProb) {
            isChanged = true;
            std::uniform_int_distribution<int> distGen(0, genotype.size() - 1);
            int j = distGen(re);
            std::swap(genotype[i], genotype[j]);
        }
    }
}

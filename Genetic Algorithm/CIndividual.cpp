#include "CIndividual.h"
CIndividual::CIndividual(int lowerBound, int upperBound, int solutionSize, std::mt19937& re){
	std::uniform_int_distribution<int> dist(lowerBound, upperBound);
	for (int i = 0; i < solutionSize; i++) {
		genotype.push_back(dist(re));
	}
}
double CIndividual::calculateFitness(const CEvaluator &evaluator) {
	this->fitnessVal = evaluator.Evaluate(this->genotype);
	return fitnessVal;
}
std::pair<CIndividual, CIndividual> CIndividual::cross(const CIndividual& pcOther, double crossProb, std::mt19937& re) const {
    std::uniform_real_distribution<double> d_dist(0, 1);
    if (crossProb < d_dist(re)) {
        return std::make_pair(*this, pcOther);
    }

    CIndividual child1 = *this;
    CIndividual child2 = pcOther;
    std::uniform_int_distribution<int> coin_flip(0, 1);

    for (size_t i = 0; i < genotype.size(); i++) {
        if (coin_flip(re) == 1) {
            std::swap(child1.genotype[i], child2.genotype[i]);
        }
    }

    return std::make_pair(child1, child2);
}

void CIndividual::localSearch(const CEvaluator& evaluator, std::mt19937& re) {
    // 1. Wybierz losowego klienta (gen)
    std::uniform_int_distribution<int> distGen(0, genotype.size() - 1);
    int customerIdx = distGen(re);

    int originalGroup = genotype[customerIdx];
    double currentFitness = fitnessVal; // Zak³adamy, ¿e jest aktualne

    // 2. Spróbuj przenieœæ go do innej losowej grupy (lub do wszystkich po kolei)
    // Dla szybkoœci sprawdŸmy np. 5 losowych innych grup
    std::uniform_int_distribution<int> distGroup(0, evaluator.GetNumGroups() - 1); // Zak³adam, ¿e masz taki getter w Evaluator

    for (int i = 0; i < 5; i++) {
        int newGroup = distGroup(re);
        if (newGroup == originalGroup) continue;

        // Podmieñ
        genotype[customerIdx] = newGroup;
        double newFitness = evaluator.Evaluate(genotype);

        if (newFitness < currentFitness) {
            // JEST LEPIEJ! Zostawiamy zmianê i aktualizujemy fitness
            fitnessVal = newFitness;
            currentFitness = newFitness;
            originalGroup = newGroup; // Teraz to jest nasza baza
        }
        else {
            // By³o gorzej, cofamy zmianê
            genotype[customerIdx] = originalGroup;
        }
    }
}
void CIndividual::mutate(double mutProb, std::mt19937 &re, int lowerBound, int upperBound) {
	std::uniform_real_distribution<double> d_dist(0, 1);
	std::uniform_int_distribution<int> int_dist(lowerBound, upperBound);
	for (int& gene : genotype) {
		if (mutProb > d_dist(re)) {
			isChanged = true;
			gene = int_dist(re);
		}
	}
}


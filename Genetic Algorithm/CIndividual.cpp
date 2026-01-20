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
std::pair<CIndividual, CIndividual> CIndividual::cross(const CIndividual& pcOther, double crossProb, std::mt19937 &re) {
	std::uniform_real_distribution<double> d_dist(0, 1);
	if (crossProb < d_dist(re)) {
		return std::make_pair(*this, pcOther);
	}
	CIndividual child1;
	CIndividual child2;
	std::uniform_int_distribution<int> int_dist(1, genotype.size()-1);
	int cutPoint = int_dist(re);
	child1.genotype.reserve(this->genotype.size());
	child2.genotype.reserve(this->genotype.size());
	child1.genotype.insert(child1.genotype.end(), this->genotype.begin(), this->genotype.begin() + cutPoint);
	child1.genotype.insert(child1.genotype.end(), pcOther.genotype.begin() + cutPoint, pcOther.genotype.end());
	child2.genotype.insert(child2.genotype.end(), pcOther.genotype.begin(), pcOther.genotype.begin() + cutPoint);
	child2.genotype.insert(child2.genotype.end(), this->genotype.begin() + cutPoint, this->genotype.end());
	child1.isChanged = true;
	child2.isChanged = true;
	return std::make_pair(child1, child2);
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


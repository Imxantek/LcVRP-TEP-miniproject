#pragma once
#include<vector>
#include <random>
#include <utility>
#include"CEvaluator.h"
#ifndef CINDIVIDUAL_H_
#define CINDIVIDUAL_H_
class CIndividual{

private:
	std::vector<int> genotype;
	double fitnessVal;
	bool isChanged = false;
public:
	CIndividual() : fitnessVal(0.0) {};
	CIndividual(int lowerBound, int upperBound, int solutionSize, std::mt19937 &re);
	CIndividual(const CIndividual& pcOther) {
		this->genotype = pcOther.genotype;
		this->fitnessVal = pcOther.fitnessVal;
	}
	double calculateFitness(const CEvaluator &Evaluator);
	std::pair<CIndividual, CIndividual> cross(const CIndividual& pcOther, double crossProb, std::mt19937 &re) const;
	void mutate(double mutProb, std::mt19937 &re, int lowerBound, int upperBound);
	const std::vector<int> &getGenotype() {
		return genotype;
	}
	double getFitness() {
		return fitnessVal;
	}
	void setGenotype(std::vector<int>& v1) {
		genotype = v1;
		isChanged = true;
	}
	void setChanged(bool val) { isChanged = val; }
	bool getChanged() { return isChanged;  }
};
#endif CINDIVIDUAL_H_
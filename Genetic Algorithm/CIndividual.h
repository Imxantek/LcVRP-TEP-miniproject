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
	bool valid;
	bool isChanged = false;
public:
	CIndividual() : fitnessVal(0.0) {};
	CIndividual(int lowerBound, int upperBound, int solutionSize, std::mt19937 &re);
	CIndividual& operator=(const CIndividual& pcOther) {
		if (this != &pcOther) {
			this->genotype = pcOther.genotype;
			this->fitnessVal = pcOther.fitnessVal;
			this->valid = pcOther.valid; 
			this->isChanged = pcOther.isChanged;   
		}
		return *this;
	}
	CIndividual& operator=(CIndividual&& pcOther) noexcept {
		if (this != &pcOther) {
			this->genotype = std::move(pcOther.genotype);
			this->fitnessVal = pcOther.fitnessVal;
			this->valid = pcOther.valid; 
			this->isChanged = pcOther.isChanged;   
		}
		return *this;
	}
	CIndividual(CIndividual&& pcOther) noexcept {
		this->genotype = std::move(pcOther.genotype);
		this->fitnessVal = pcOther.fitnessVal;
		this->valid = pcOther.valid;
		this->isChanged = pcOther.isChanged;
	}
	CIndividual(const CIndividual& pcOther) {
		this->genotype = pcOther.genotype;
		this->fitnessVal = pcOther.fitnessVal;
		this->valid = pcOther.valid; 
		this->isChanged = pcOther.isChanged;   
	}
	double calculateFitness(const CEvaluator &Evaluator);
	std::pair<CIndividual, CIndividual> cross(const CIndividual& pcOther, double crossProb, std::mt19937 &re) const;
	void aggresiveMutate(double mutProb, std::mt19937& re, int lowerBound, int upperBound) {
		std::uniform_real_distribution<double> d_dist(0, 1);
		std::uniform_int_distribution<int> int_dist(lowerBound, upperBound - 1);
		for (int& gene : genotype) {
			if (mutProb > d_dist(re)) {
				isChanged = true;
				gene = int_dist(re);
			}
		}
	};
	void mutate(double mutProb, std::mt19937 &re, int lowerBound, int upperBound);
	const std::vector<int> &getGenotype() {
		return genotype;
	}
	double getFitness() {
		return fitnessVal;
	}
	void setGenotype(std::vector<int>& v1) {
		genotype = v1;
		isChanged=true;
	}
	void setChanged(bool val) { isChanged = val; }
	bool getChanged() { return isChanged;  }
	void setFitness(double val) { fitnessVal = val; }
	void localSearch(const CEvaluator& evaluator, std::mt19937& re);
	bool isValid() { return valid; }

};
#endif CINDIVIDUAL_H_
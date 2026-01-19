#pragma once
#include<string>
#ifndef CEVALUATOR_H_
#define CEVALUATOR_H_
class CEvaluator{

private:
	void validateConstraints();
public:
	CEvaluator();
	~CEvaluator();
	CEvaluator(CEvaluator& pcOther);
	void loadInstance(std::string path);
	double evaluate(const std::vector<int>& genotype) const;
};
#endif CEVALUATOR_H_
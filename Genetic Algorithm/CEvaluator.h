#pragma once
#ifndef CEVALUATOR_H_
#define CEVALUATOR_H_
#include "ProblemData.h"
#include <vector>
#include <string>
#include <utility> 
class CEvaluator {
public:
    CEvaluator(const ProblemData& data, int num_groups);

    double Evaluate(const vector<int>& solution) const;
    double Evaluate(const int* solution) const;

private:
    const ProblemData& data;
    int groups;
    int customers;
    const double WRONG_VAL = -1.0;
    
    double calculateRouteCost(const std::vector<int>& route) const;
    bool isValid(const vector<int>& grouping) const;
    bool ValidateConstraints() const;
    void BuildRoutes(const vector<int>& grouping, vector<vector<int>>& routes) const;
};

#endif // CEVALUATOR_H_
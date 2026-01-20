#pragma once
#ifndef CEVALUATOR_H_
#define CEVALUATOR_H_
#include "ProblemData.h"
#include <vector>
#include <string>
#include <utility> 
class CEvaluator {
public:
    CEvaluator(const ProblemData& problemData, int num_groups);
    double Evaluate(const vector<int>& solution) const;
    bool isValid(const vector<int>& solution) const;
    bool validateConstraints() const;
    void build(std::vector<std::vector<int>>& routes, const vector<int>& solution) const;
    double calculateRouteCost(const std::vector<int>& route) const;
    int GetNumGroups() { return num_groups; }
    int GetNumCustomers() { return problemData.GetNumCustomers(); }
private:
    ProblemData problemData;
    int num_groups;
    double WRONG_VAL = 1000000.0;
};
#endif CEVALUATOR_H_
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
    std::pair<double, bool> Evaluate(const vector<int>& solution) const;
    bool isValid(const vector<int>& solution) const;
    void build(std::vector<std::vector<int>>& routes, const vector<int>& solution) const;
    std::pair<double, bool> calculateRouteCost(const std::vector<int>& route) const;
    int GetNumGroups() const { return num_groups; }
    int GetNumCustomers() const { return problemData.GetNumCustomers(); }
private:
    ProblemData problemData;
    int num_groups;
    double WRONG_VAL; 
    mutable std::vector<std::vector<int>> routes_cache;
};
#endif CEVALUATOR_H_
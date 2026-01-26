#include "CEvaluator.h"
#include <algorithm>
#include <cmath>
#include <numeric>
#include <ranges>
#include <stdexcept>

CEvaluator::CEvaluator(const ProblemData& problemData, int num_groups) :
	problemData(problemData), num_groups(num_groups)
{
	routes_cache.resize(num_groups);
	int avg_cities = problemData.GetNumCustomers() / num_groups + 2;
	for (auto& route : routes_cache) {
		route.reserve(avg_cities);
	}
	double max_weight = 0.0;
	const auto& weights = problemData.GetEdgeWeights();
	for (const auto& row : weights) {
		for (double w : row) {
			if (w > max_weight) {
				max_weight = w;
			}
		}
	}
	WRONG_VAL= max_weight*200;
};


std::pair<double,bool> CEvaluator::Evaluate(const vector<int>& solution) const {
	for (auto& route : routes_cache) {
		route.clear();
	}
	build(routes_cache, solution);

	std::pair<double, bool> total_cost = std::make_pair(0, true);

	for (const auto& route : routes_cache) {
		const std::pair<double, bool> routeCost = calculateRouteCost(route);
		total_cost.first += routeCost.first;
		if (!routeCost.second) {
			total_cost.second = false;
		}
	}
	return total_cost;
}

bool CEvaluator::isValid(const vector<int>& solution) const {
	for (int i : solution) {
		if (i<0 || i>num_groups - 1) {
			return false;
		}
	}
	return true;
}

//bool CEvaluator::validateConstraints() const {
//	int depotID = problemData.GetDepot() - 1;
//	int capacity = problemData.GetCapacity();
//	const vector<int>& demands = problemData.GetDemands();
//	for (int i = 2; i <= problemData.GetDimension(); i++) {
//		int customerID = i - 1;
//		if (demands[customerID] > capacity) return false;
//	}
//	return true;
//}

void CEvaluator::build(std::vector<std::vector<int>>& routes, const std::vector<int>& solution) const {
	const vector<int>& permutation = problemData.GetPermutation();
	size_t permSize = permutation.size();
	for (size_t i = 0; i < permSize; i++) {
		int idx = permutation[i] - 2;
		if (idx >= 0 && idx < problemData.GetNumCustomers()) {
			int group = solution[idx];
			routes[group].push_back(idx + 2);
		}
	}
}

std::pair<double, bool> CEvaluator::calculateRouteCost(const std::vector<int>& route) const {
	if (route.empty()) {
		return std::make_pair(0.0, true);
	}
	int depotID = problemData.GetDepot() - 1;
	double routeCost = 0.0;
	double capacity = problemData.GetCapacity();
	

	int weight = 0;
	const size_t size = route.size();
	const vector<int>& demands = problemData.GetDemands();

	routeCost += problemData.CalculateDistance(depotID, route[0]-1);
	weight += demands[route[0]-1];

	for (size_t i = 0; i < size - 1; i++) {
		weight += demands[route[i+1]-1];
		routeCost += problemData.CalculateDistance(route[i]-1, route[i + 1]-1);
	}

	routeCost += problemData.CalculateDistance(route[size - 1]-1, depotID);
	if(capacity < weight) {
		return std::make_pair((weight - capacity) * WRONG_VAL + routeCost, false);
	}
	return std::make_pair(routeCost, true);
}
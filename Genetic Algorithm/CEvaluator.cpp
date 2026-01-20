#include "CEvaluator.h"
CEvaluator::CEvaluator(const ProblemData& problemData, int num_groups) :
	problemData(problemData), num_groups(num_groups) {};

double CEvaluator::Evaluate(const vector<int>& solution) const{
	if (solution.size() != problemData.GetNumCustomers() || !isValid(solution) || !validateConstraints()) {
		return WRONG_VAL;
	}
	std::vector<std::vector<int>> routes;
	build(routes, solution);
	double cost = 0.0;
	for (const auto &a: routes) {
		double routeCost = calculateRouteCost(a);
		if (routeCost < 0) {
			return WRONG_VAL;
		}
		cost += routeCost;
	}
	return cost;
}
bool CEvaluator::isValid(const vector<int>& solution) const{
	for (int i : solution) {
		if (i<0 || i>num_groups - 1) {
			return false;
		}
	}
	return true;
}
bool CEvaluator::validateConstraints() const {
	int depotID = problemData.GetDepot() - 1;
	int capacity = problemData.GetCapacity();
	const vector<int>& demands = problemData.GetDemands();
	for (int i = 2; i <= problemData.GetDimension(); i++) {
		int customerID = i - 1;
		if (demands[customerID] > capacity) return false;
		if (problemData.HasDistanceConstraint()) {
			double distToDepot = problemData.CalculateDistance(depotID, customerID);
			double maxDist = problemData.GetDistance();
			if (distToDepot < 0.0) return false;
			if (2 * distToDepot > maxDist) return false;
		}
	}
	return true;
}
void CEvaluator::build(std::vector<std::vector<int>> &routes, const std::vector<int> &solution) const {
	routes.clear();
	routes.resize(num_groups);
	const vector<int>& permutation = problemData.GetPermutation();
	for (int i = 0; i < permutation.size(); i++) {
		int idx = permutation[i] - 2;
		if (idx >= 0 && idx < problemData.GetNumCustomers()) {
			int group = solution[idx];
			routes[group].push_back(idx+2);
		}
	}
}
double CEvaluator::calculateRouteCost(const std::vector<int> &route) const{
	if (route.empty()) {
		return 0.0;
	}
	int depotID = problemData.GetDepot()-1;
	double routeCost = 0.0;
	double capacity = problemData.GetCapacity();
	double maxDist;
	if (problemData.HasDistanceConstraint()) {
		maxDist = problemData.GetDistance();
	}
	else {
		maxDist = -1.0;
	}
	const vector<int>& demands = problemData.GetDemands();
	
	int currentLoad = 0;
	double currentDistance = 0.0;
	int currentPos = depotID;
	for (int i = 0; i < route.size(); i++) {
		int customerID = route[i] - 1;
		int customerDemand = demands[customerID];
		if (currentLoad + customerDemand > capacity) {
			double distDepot = problemData.CalculateDistance(depotID, currentPos);
			if (distDepot < 0.0) {
				return WRONG_VAL;
			}
			currentDistance += distDepot;
			routeCost += currentDistance;

			currentLoad = 0;
			currentDistance = 0.0;
			currentPos = depotID;
		}
		double distCustomer = problemData.CalculateDistance(currentPos, customerID);
		if (distCustomer < 0.0) return WRONG_VAL;
		if (maxDist != -1) {
			double distCustomerToDepot = problemData.CalculateDistance(customerID, depotID);
			if (distCustomerToDepot < 0.0) return WRONG_VAL;
			if (currentDistance + distCustomer + distCustomerToDepot > maxDist) {
				double distDepot = problemData.CalculateDistance(currentPos, depotID);
				if (distDepot < 0.0) return WRONG_VAL;
				currentDistance += distDepot;
				routeCost += currentDistance;
				
				currentLoad = 0;
				currentDistance = 0.0;
				currentPos = depotID;

				distCustomer = problemData.CalculateDistance(depotID, customerID);
				if (distCustomer < 0.0) return WRONG_VAL;
			}
		}
		currentDistance += distCustomer;
		currentLoad += customerDemand;
		currentPos = customerID;
	}
	double distDepot = problemData.CalculateDistance(currentPos, depotID);
	if (distDepot < 0.0) return WRONG_VAL;
	currentDistance += distDepot;
	routeCost += currentDistance;
	return routeCost;
}

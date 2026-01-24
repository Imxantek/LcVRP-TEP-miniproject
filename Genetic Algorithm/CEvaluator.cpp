#include "CEvaluator.h"
#include <limits> // do sprawdzenia max double

CEvaluator::CEvaluator(const ProblemData& problemData, int num_groups) :
	problemData(problemData), num_groups(num_groups)
{
	routes_cache.resize(num_groups);
	int avg_cities = problemData.GetNumCustomers() / num_groups + 2;
	for (auto& route : routes_cache) {
		route.reserve(avg_cities);
	}
};
double CEvaluator::Evaluate(const vector<int>& solution) const{
	if (solution.size() != problemData.GetNumCustomers() || !isValid(solution)) {
		return WRONG_VAL;
	}
	for (auto& route : routes_cache) {
		route.clear();
	}
	build(routes_cache, solution);

	double cost = 0.0;
	for (const auto& route : routes_cache) {
		double routeCost = calculateRouteCost(route);
		if (routeCost == WRONG_VAL) return WRONG_VAL;
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
void CEvaluator::build(std::vector<std::vector<int>>& routes, const std::vector<int>& solution) const {
	// USUNIÊTO: routes.clear();  <-- To niszczy³o cache
	// USUNIÊTO: routes.resize(num_groups); <-- To alokowa³o od nowa

	// Zak³adamy, ¿e 'routes' ma ju¿ odpowiedni rozmiar i wyczyszczone œrodki
	// (gwarantuje to metoda Evaluate)

	const vector<int>& permutation = problemData.GetPermutation();
	// U¿ywamy size_t dla wydajnoœci i unikniêcia warningów
	size_t permSize = permutation.size();

	for (size_t i = 0; i < permSize; i++) {
		int idx = permutation[i] - 2;
		// Opcjonalnie: usuniêcie tego ifa w wersji Release, jeœli dane s¹ pewne
		if (idx >= 0 && idx < problemData.GetNumCustomers()) {
			int group = solution[idx];
			// Tu po prostu wrzucamy do gotowego kube³ka
			routes[group].push_back(idx + 2);
		}
	}
}
double CEvaluator::calculateRouteCost(const std::vector<int>& route) const {
	if (route.empty()) {
		return 0.0;
	}
	int depotID = problemData.GetDepot() - 1;
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
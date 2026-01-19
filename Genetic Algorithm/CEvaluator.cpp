#include "CEvaluator.h"
CEvaluator::CEvaluator(const ProblemData& data, int groups) :data(data), groups(groups), customers(data.GetNumCustomers()) {};

double CEvaluator::Evaluate(const vector<int>& solution) const {
	if (solution.size() != static_cast<int>(customers)) {
		return WRONG_VAL;
	}
	if (!ValidateConstraints()) {
		return WRONG_VAL;
	}
	if (!isValid(solution)) {
		return WRONG_VAL;
	}
	vector<vector<int>> routes;
	BuildRoutes(solution, routes);
	double total_cost = 0.0;
	for (const auto& route : routes) {
		double route_cost = calculateRouteCost(route);
		if (route_cost < 0.0) {
			return WRONG_VAL;
		}
		total_cost += route_cost;
	}
	return total_cost;
}

void CEvaluator::BuildRoutes(const vector<int>& grouping, vector<vector<int>>& routes) const {
	routes.clear();
	routes.resize(groups);
	const vector<int>& permutation = data.GetPermutation();
	for (int i = 0; i < permutation.size(); ++i) {
		int customer_id = permutation[i];
		int customer_index = customer_id - 2;
		if (customer_index >= 0 && customer_index < customers) {
			int group = grouping[customer_index];
			routes[group].push_back(customer_id);
		}
	}
}

bool CEvaluator::isValid(const vector<int>& grouping) const {
	for (int group : grouping) {
		if (group < 0 || group > groups-1) {
			return false;
		}
	}
	return true;
}

bool CEvaluator::ValidateConstraints() const {
	int depot = data.GetDepot();
	int depot_index = depot - 1; 
	const vector<int>& demands = data.GetDemands();
	int capacity = data.GetCapacity();

	for (int customer_id = 2; customer_id <= data.GetDimension(); ++customer_id) {
		int customer_index = customer_id - 1;
		if (demands[customer_index] > capacity) {
			return false;
		}
		if (data.HasDistanceConstraint()) {
			double max_distance = data.GetDistance();
			double dist_to_depot = data.CalculateDistance(depot_index, customer_index);
			if (dist_to_depot < 0.0) {
				return false;
			}
			if (dist_to_depot * 2.0 > max_distance) {
				return false;
			}
		}
	}
	return true;
}

double CEvaluator::calculateRouteCost(const vector<int>& route) const {
	if (route.empty()) {
		return 0.0;
	}
	double total_cost = 0.0;
	int depot = data.GetDepot();
	int depot_index = depot - 1; 
	int capacity = data.GetCapacity();
	double max_distance = data.HasDistanceConstraint() ? data.GetDistance() : -1.0;
	const vector<int>& demands = data.GetDemands();
	int current_load = 0;
	double current_distance = 0.0;
	int last_position_index = depot_index;

	for (int i = 0; i < route.size(); ++i) {
		int customer_id = route[i];
		int customer_index = customer_id - 1;
		int customer_demand = demands[customer_index];
		if (current_load + customer_demand > capacity) {
			double dist_to_depot = data.CalculateDistance(last_position_index, depot_index);
			if (dist_to_depot < 0.0) return WRONG_VAL;
			current_distance += dist_to_depot;
			total_cost += current_distance;
			current_load = 0;
			current_distance = 0.0;
			last_position_index = depot_index;
		}
		double dist_to_customer = data.CalculateDistance(last_position_index, customer_index);
		if (dist_to_customer < 0.0) return WRONG_VAL;
		if (max_distance > 0.0) {
			double dist_back_to_depot = data.CalculateDistance(customer_index, depot_index);
			if (dist_back_to_depot < 0.0) return WRONG_VAL;
			if (current_distance + dist_to_customer + dist_back_to_depot > max_distance) {
				double dist_to_depot = data.CalculateDistance(last_position_index, depot_index);
				if (dist_to_depot < 0.0) return WRONG_VAL;
				current_distance += dist_to_depot;
				total_cost += current_distance;
				current_load = 0;
				current_distance = 0.0;
				last_position_index = depot_index;
				dist_to_customer = data.CalculateDistance(depot_index, customer_index);
				if (dist_to_customer < 0.0) return WRONG_VAL;
			}
		}
		current_distance += dist_to_customer;
		current_load += customer_demand;
		last_position_index = customer_index;
	}
	double dist_to_depot = data.CalculateDistance(last_position_index, depot_index);
	if (dist_to_depot < 0.0) return WRONG_VAL;
	current_distance += dist_to_depot;
	total_cost += current_distance;
	return total_cost;
}


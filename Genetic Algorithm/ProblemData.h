#pragma once
#ifndef PROBLEMDATA_H_
#define PROBLEMDATA_H_
#include <string>
#include <vector>
#include<cmath>
#include <stdexcept>
using namespace std;

struct Coordinate {
	double x;
	double y;

	Coordinate() : x(0.0), y(0.0) {}
	Coordinate(double x, double y) : x(x), y(y) {}
};

class ProblemData {
public:
	ProblemData();

	string GetName() const { return name; }
	int GetDimension() const { return dimension; }
	int GetCapacity() const { return capacity; }
	string GetEdgeWeightType() const { return edge_weight_type; }
	int GetDepot() const { return depot; }
	int GetNumCustomers() const { return dimension - 1; } 

	vector<Coordinate>& GetCoordinates() { return coordinates; }
	const vector<int>& GetDemands() const { return demands; }
	const vector<int>& GetPermutation() const { return permutation; }
	const vector<vector<double>>& GetEdgeWeights() const { return edge_weights; }
	vector<int>& AccessPermutation() { return permutation; }
	vector<int>& AccessDemands() { return demands; }
	void SetName(const string& name) { this->name = name; }
	void SetDimension(int dimension) {
		this->dimension = dimension;
		this->coordinates.resize(dimension);
		this->demands.resize(dimension);
	}
	void SetCapacity(int capacity) { this->capacity = capacity; }
	void SetEdgeWeightType(const string& type) { edge_weight_type = type; }
	void SetDepot(int depot) { this->depot = depot; }

	void SetCoordinates(const vector<Coordinate>& coordinates) { this->coordinates = coordinates; }
	void SetDemands(const vector<int>& demands) { this->demands = demands; }
	void SetPermutation(const vector<int>& permutation) { this->permutation = permutation; }
	void SetEdgeWeights(const vector<vector<double>>& edge_weights) { this->edge_weights = edge_weights; }


	double CalculateDistance(int i, int j) const;
	void BuildEdgeWeightMatrix();

private:
	std::vector<std::vector<double>> distanceMatrix;
	string name;
	int dimension; 
	int capacity; 
	double distance; 
	bool has_distance_constraint;
	string edge_weight_type; 
	int depot; 

	vector<Coordinate> coordinates; 
	vector<int> demands; 
	vector<int> permutation; 
	vector<vector<double>> edge_weights; 
};
#endif PROBLEMDATA_H_
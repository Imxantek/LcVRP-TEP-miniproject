#include "ProblemData.h"

ProblemData::ProblemData()
    : dimension(0),
    capacity(0),
    distance(0.0),
    has_distance_constraint(false),
    depot(1) {
}

double ProblemData::CalculateDistance(int i, int j) const {
    /*if (i < 0 || i >= dimension || j < 0 || j >= dimension) {
        throw std::invalid_argument("The edge (" + std::to_string(i) + "," + std::to_string(j) + ") doesn't exist in matrix!");
    }
    if (i == j) return 0.0;

    if (edge_weight_type == "EUC_2D") {

        if (coordinates.size() != static_cast<size_t>(dimension)) {
            throw  std::invalid_argument("size of coordinates must be the same as dimension!");
        }

        if (i > j) {
            return edge_weights[i][j];
        }
        return edge_weights[j][i];
    }
    if (edge_weight_type == "EXPLICIT") {
        if (edge_weights.empty() || i >= static_cast<int>(edge_weights.size()) || j >= static_cast<int>(edge_weights[i].size())) {
            throw std::invalid_argument("The edge (" + std::to_string(i) + "," + std::to_string(j) + ") doesn't exist in matrix!");
        }
        return edge_weights[i][j];
    }
    throw std::domain_error("This type of edge[ " + edge_weight_type + " ] is unsupported!");*/
    return edge_weights[i][j];
}
void ProblemData::BuildEdgeWeightMatrix() {
    if (edge_weight_type == "EUC_2D") {
        if (coordinates.size() != static_cast<size_t>(dimension)) {
            return;
        }

        edge_weights.resize(dimension);
        for (int i = 0; i < dimension; ++i) {
            edge_weights[i].resize(dimension);
            for (int j = 0; j < dimension; ++j) {
                if (i == j) {
                    edge_weights[i][j] = 0.0;
                }
                else {
                    double dx = coordinates[i].x - coordinates[j].x;
                    double dy = coordinates[i].y - coordinates[j].y;
                    edge_weights[i][j] = sqrt(dx * dx + dy * dy);
                }
            }
        }
    }
}

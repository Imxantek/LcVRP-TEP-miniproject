#include "ProblemData.h"

ProblemData::ProblemData()
    : dimension_(0),
    capacity_(0),
    distance_(0.0),
    has_distance_constraint_(false),
    depot_(1) {
}

double ProblemData::CalculateDistance(int i, int j) const {
    // 1. Jeœli zapytanie jest niepoprawne - odrzuæ
    if (i < 0 || i >= dimension_ || j < 0 || j >= dimension_) {
        return WRONG_VAL;
    }

    // 2. OPTYMALIZACJA (TEGO BRAKOWA£O):
    // Jeœli macierz jest pusta, ka¿emy j¹ zbudowaæ (tylko raz w historii programu!)
    if (edge_weights_.empty()) {
        const_cast<ProblemData*>(this)->BuildEdgeWeightMatrix();
    }

    // 3. Pobranie gotowego wyniku z pamiêci (B£YSKAWICZNE)
    if (!edge_weights_.empty()) {
        return edge_weights_[i][j];
    }

    // 4. Stary, wolny sposób (tylko awaryjnie)
    if (edge_weight_type_ == "EUC_2D") {
        double dx = coordinates_[i].x - coordinates_[j].x;
        double dy = coordinates_[i].y - coordinates_[j].y;
        return std::sqrt(dx * dx + dy * dy);
    }

    return WRONG_VAL;
}
void ProblemData::BuildEdgeWeightMatrix() {
    if (edge_weight_type_ == "EUC_2D") {
        if (coordinates_.size() != static_cast<size_t>(dimension_)) {
            return;
        }

        edge_weights_.resize(dimension_);
        for (int i = 0; i < dimension_; ++i) {
            edge_weights_[i].resize(dimension_);
            for (int j = 0; j < dimension_; ++j) {
                if (i == j) {
                    edge_weights_[i][j] = 0.0;
                }
                else {
                    double dx = coordinates_[i].x - coordinates_[j].x;
                    double dy = coordinates_[i].y - coordinates_[j].y;
                    edge_weights_[i][j] = sqrt(dx * dx + dy * dy);
                }
            }
        }
    }
}

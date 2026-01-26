#include "ProblemLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>


ProblemLoader::ProblemLoader(const string& folder_name, const string& instance_name)
    : folder_name_(folder_name),
    instance_name_(instance_name)
{
    base_path_ = "data/lcvrp/" + folder_name_ + "/";
}

ProblemData ProblemLoader::LoadProblem() {
    ProblemData problem_data;
    string file_path = base_path_ + instance_name_ + ".lcvrp";
    ParseLcVrpFile(file_path, problem_data);
    if (problem_data.GetEdgeWeightType() == "EUC_2D") {
        problem_data.BuildEdgeWeightMatrix();
    }
    return problem_data;
}

void ProblemLoader::ParseLcVrpFile(const string& file_path, ProblemData& problem_data) {
    ifstream file(file_path);
    if (!file.is_open()) {
        cout << "Error: Cannot open file: " << file_path << endl;
        exit(1);
    }
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;


        if (line.find("NAME") != string::npos) {
			ParseName(line, problem_data);
        }
        else if (line.find("DIMENSION") != string::npos) {
			ParseDimension(line, problem_data);
        }
        else if (line.find("CAPACITY") != string::npos) {
			ParseCapacity(line, problem_data);
        }
        
        else if (line.find("EDGE_WEIGHT_TYPE") != string::npos) {
            ParseEdgeWeightType(line, problem_data);
        }
        else if (line.find("EDGE_WEIGHT_SECTION") != string::npos) {
            ParseEdgeWeightSection(file, problem_data);
        }
        else if (line.find("NODE_COORD_SECTION") != string::npos) {
            ParseNodeCoordSection(file, problem_data);
        }
        else if (line.find("DEMAND_SECTION") != string::npos) {
            ParseDemandSection(file, problem_data);
        }
        else if (line.find("DEPOT_SECTION") != string::npos) {
            ParseDepotSection(file, problem_data);
        }
        else if (line.find("PERMUTATION") != string::npos) {
			ParsePermutation(line, problem_data);
        }
        else if (line.find("EOF") != string::npos) {
            if (problem_data.GetPermutation().empty() || problem_data.GetCapacity() == 0 || problem_data.GetCoordinates().empty() || problem_data.GetDemands().empty())
                throw std::invalid_argument("Missing data!");
            break;
        }
    }

    file.close();
}

void ProblemLoader::ParseName(const string & line, ProblemData & problem_data) {
    size_t colon_pos = line.find(':');
    if (colon_pos != string::npos) {
        string name = line.substr(colon_pos + 1);
        size_t start = name.find_first_not_of(" \t\r\n");
        if (start != string::npos) {
            name = name.substr(start);
            size_t end = name.find_last_not_of(" \t\r\n");
            name = name.substr(0, end + 1);
        }
        problem_data.SetName(name);
    }
}

void ProblemLoader::ParseDimension(const string& line, ProblemData& problem_data) {
    size_t colon_pos = line.find(':');
    if (colon_pos != string::npos) {
        int dimension = stoi(line.substr(colon_pos + 1));
        problem_data.SetDimension(dimension);
    }
}

void ProblemLoader::ParseCapacity(const string& line, ProblemData& problem_data) {
    size_t colon_pos = line.find(':');
    if (colon_pos != string::npos) {
        int capacity = stoi(line.substr(colon_pos + 1));
        problem_data.SetCapacity(capacity);
    }
}

void ProblemLoader::ParseEdgeWeightType(const string& line, ProblemData& problem_data) {
    size_t colon_pos = line.find(':');
    if (colon_pos != string::npos) {
        string type = line.substr(colon_pos + 1);
        size_t start = type.find_first_not_of(" \t\r\n");
        if (start != string::npos) {
            type = type.substr(start);
            size_t end = type.find_last_not_of(" \t\r\n");
            type = type.substr(0, end + 1);
        }
        problem_data.SetEdgeWeightType(type);
    }
}

void ProblemLoader::ParseEdgeWeightSection(ifstream& file, ProblemData& problem_data) {
    int dimension = problem_data.GetDimension();
    vector<vector<double>> edge_weights(dimension, vector<double>(dimension, 0.0));
    for (int i = 1; i < dimension; ++i) {
        for (int j = 0; j < i; ++j) {
            double weight;
            file >> weight;
            edge_weights[i][j] = weight;
            edge_weights[j][i] = weight;
        }
    }
    problem_data.SetEdgeWeights(edge_weights);
}



void ProblemLoader::ParseNodeCoordSection(ifstream& file, ProblemData& problem_data) {
    int dimension = problem_data.GetDimension();
    vector<Coordinate> coordinates(dimension);

    for (int i = 0; i < dimension; ++i) {
        int node_id;
        double x, y;
        file >> node_id >> x >> y;
        coordinates[node_id - 1] = Coordinate(x, y);
    }

    problem_data.SetCoordinates(coordinates);
}

void ProblemLoader::ParseDemandSection(ifstream& file, ProblemData& problem_data) {
    int dimension = problem_data.GetDimension();
    vector<int> demands(dimension);

    for (int i = 0; i < dimension; ++i) {
        int node_id, demand;
        file >> node_id >> demand;
        demands[node_id - 1] = demand;
    }

    problem_data.SetDemands(demands);
}

void ProblemLoader::ParseDepotSection(ifstream& file, ProblemData& problem_data) {
    int depot;
    file >> depot;
    problem_data.SetDepot(depot);
    int terminator;
    file >> terminator;
}

void ProblemLoader::ParsePermutation(const string& line, ProblemData& problem_data) {
    size_t colon_pos = line.find(':');
    if (colon_pos != string::npos) {
        vector<int> permutation;
        string perm_data = line.substr(colon_pos + 1);
        istringstream iss(perm_data);
        int customer_id;
        while (iss >> customer_id) {
            permutation.push_back(customer_id);
        }
        problem_data.SetPermutation(permutation);
    }
}
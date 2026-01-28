#include "ProblemLoader.h"
#include <iostream>
#include <sstream>
#include <algorithm>

ProblemLoader::ProblemLoader(string folder, string instance) {
    fullPath_ = "data/lcvrp/" + folder + "/" + instance + ".lcvrp";
}

ProblemData ProblemLoader::Load() {
    ifstream file(fullPath_);
    if (!file.is_open()) {
        throw runtime_error("Critical Error: Unable to open file: " + fullPath_);
    }

    ProblemData data;
    string line;
    string currentSection = "";

    while (getline(file, line)) {
        if (line.empty()) continue;
        replace(line.begin(), line.end(), ':', ' ');
        stringstream ss(line);
        string key;
        ss >> key; 
        if (key == "NAME") {
            string name; 
            ss >> name;
            data.SetName(name);
        }
        else if (key == "DIMENSION") {
            int dim; 
            ss >> dim;
            data.SetDimension(dim);
        }
        else if (key == "CAPACITY") {
            int cap;
            ss >> cap;
            data.SetCapacity(cap);
        }
        else if (key == "EDGE_WEIGHT_TYPE") {
            string type; 
            ss >> type;
            data.SetEdgeWeightType(type);
        }
        else if (key == "PERMUTATION") {
            int val;
            while (ss >> val) data.AccessPermutation().push_back(val);
        }
        else if (key == "NODE_COORD_SECTION") {
            currentSection = "COORD";
            continue; 
        }
        else if (key == "DEMAND_SECTION") {
            currentSection = "DEMAND";
            continue;
        }
        else if (key == "DEPOT_SECTION") {
            currentSection = "DEPOT";
            continue;
        }
        else if (key == "EOF") {
            break;
        }
        else {
            if (currentSection == "COORD") {
                int id = stoi(key);
                double x, y;
                ss >> x >> y;
                if (id - 1 < data.GetDimension()) {
                    data.GetCoordinates()[id - 1] = { x, y };
                }
            }
            else if (currentSection == "DEMAND") {
                int id = stoi(key);
                int demand;
                ss >> demand;
                if (id - 1 < data.GetDimension()) {
                    data.AccessDemands()[id - 1] = demand;
                }
            }
            else if (currentSection == "DEPOT") {
                int depotId = stoi(key);
                if (depotId != -1) { 
                    data.SetDepot(depotId);
                }
            }
        }
    }

    file.close();

    
    if (data.GetEdgeWeightType() == "EUC_2D") {
        data.BuildEdgeWeightMatrix(); 
    }

    return data;
}

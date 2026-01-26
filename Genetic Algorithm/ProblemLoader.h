#pragma once

#include "ProblemData.h"
#include <string>

using namespace std;

class ProblemLoader {
public:
	ProblemLoader(const string& folder_name, const string& instance_name);
	ProblemData LoadProblem();

private:
	string folder_name_;
	string instance_name_;
	string base_path_;
	void ParseName(const string& line, ProblemData& problem_data);
	void ParseDimension(const string& line, ProblemData& problem_data);
	void ParseCapacity(const string& line, ProblemData& problem_data);
	void ParseEdgeWeightType(const string& line, ProblemData& problem_data);
	void ParseLcVrpFile(const string& file_path, ProblemData& problem_data);
	void ParseEdgeWeightSection(ifstream& file, ProblemData& problem_data);
	void ParseNodeCoordSection(ifstream& file, ProblemData& problem_data);
	void ParseDemandSection(ifstream& file, ProblemData& problem_data);
	void ParseDepotSection(ifstream& file, ProblemData& problem_data);
	void ParsePermutation(const string& line, ProblemData& problem_data);
};


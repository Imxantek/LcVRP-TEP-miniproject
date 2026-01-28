#pragma once
#ifndef PROBLEMLOADER_H_
#define PROBLEMLOADER_H_

#include "ProblemData.h"
#include <string>
#include <fstream>
#include <functional>
#include <map>

class ProblemLoader {
public:
    ProblemLoader(std::string folder, std::string instance);
    ProblemData Load();

private:
    std::string fullPath_;
};

#endif // !PROBLEMLOADER_H_
#pragma once

#include <string>
#include <stack>

#include "matrix.hpp"
#include "screen.hpp"

void parse_file (std::string filename, std::stack<matrix>& cstack, edgelist& E, trianglelist& T, picture& s, std::vector<std::vector<double>>& zbuffer);

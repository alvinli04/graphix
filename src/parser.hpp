#pragma once

#include <string>

#include "matrix.hpp"
#include "screen.hpp"

void parse_file (std::string filename, matrix& M, edgelist& E, picture& s);

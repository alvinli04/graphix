#pragma once

#include <string>
#include <stack>

#include "matrix.hpp"
#include "screen.hpp"

void parse_file (std::string filename, std::stack<matrix>& cstack, edgelist& E, trianglelist& T, picture& s, std::vector<std::vector<double>>& zbuffer,
                 const color& ambient, std::vector<light>& lights, 
                 double ka_r, double kd_r, double ks_r,
                 double ka_g, double kd_g, double ks_g,
                 double ka_b, double kd_b, double ks_b);

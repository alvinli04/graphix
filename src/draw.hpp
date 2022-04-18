#pragma once

#include "matrix.hpp"
#include "screen.hpp"

void draw_line (int x0, int y0, double z0, int x1, int y1, double z1, picture& p, const color& c, std::vector<std::vector<double>>& zbuffer);
void draw_lines (edgelist& points, picture& p, const color& c, std::vector<std::vector<double>>& zbuffer);
void draw_lines (trianglelist& points, picture& p, const color& c, std::vector<std::vector<double>>& zbuffer);

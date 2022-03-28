#pragma once

#include "matrix.hpp"
#include "screen.hpp"

void draw_line (int x0, int y0, int x1, int y1, picture& p, const color& c);
void draw_lines (edgelist& points, picture& p, const color& c);
void draw_lines (trianglelist& points, picture& p, const color& c);

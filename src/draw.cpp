#include <algorithm>
#include <iostream>

#include "matrix.hpp"
#include "screen.hpp"
#include "draw.hpp"


// Draws a line from (x0, y0) to (x1, y1)
void draw_line (int x0, int y0, int x1, int y1, picture& p, const color& c) {
	// starting point is always left of the ending point
	if (x0 > x1) {
		std::swap (x0, x1);
		std::swap (y0, y1);
	}
	
	int x = x0;
	int y = y0;
	int A = y1 - y0;
	int B = x0 - x1;

	if (y1 >= y0 && A <= B * -1) {
		// octant 1
		int D = 2 * A + B;

		while (x <= x1) {
			p[y][x].set(c);
			if (D > 0) {
				D += 2 * B;
				++y;
			}

			++x;
			D += 2 * A;
		}
	} else if (y1 >= y0) {
		// octant 2
		int D = A + 2 * B;

		while (y <= y1) {
			p[y][x].set(c);
			if (D < 0) {
				D += 2 * A;
				++x;
			}

			++y;
			D += 2 * B;
		}
	} else if (A >= B) {
		// octant 8
		int D = 2 * A - B;

		while (x <= x1) {
			p[y][x].set(c);
			if (D < 0) {
				D -= 2 * B;
				--y;
			}
			++x;
			D += 2 * A;
		}
	} else {
		// octant 7
		int D = A - 2 * B;

		while (y >= y1) {
			p[y][x].set(c);
			if (D > 0) {
				D += 2 * A;
				++x;
			}

			--y;
			D -= 2 * B;
		}
	}
}


void draw_lines (edgelist& points, picture& p, const color& c) {
	for (int i = 0; i < points.cols - 1; i += 2) {
		draw_line (points[0][i], points[1][i], points[0][i+1], points[1][i+1], p, c);
	}
}

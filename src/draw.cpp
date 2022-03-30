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
			if (0 <= y && y < p.height && 0 <= x && x < p.width) {
				p[y][x].set(c);
			}
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
			if (0 <= y && y < p.height && 0 <= x && x < p.width) {
				p[y][x].set(c);
			}
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
			if (0 <= y && y < p.height && 0 <= x && x < p.width) {
				p[y][x].set(c);
			}
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
			if (0 <= y && y < p.height && 0 <= x && x < p.width) {
				p[y][x].set(c);
			}
			if (D > 0) {
				D += 2 * A;
				++x;
			}

			--y;
			D -= 2 * B;
		}
	}
}

// Draws lines from an edge list
void draw_lines (edgelist& points, picture& p, const color& c) {
	for (int i = 0; i < points.cols; i += 2) {
		draw_line (points[0][i], points[1][i], points[0][i+1], points[1][i+1], p, c);
	}
}

// Draws lines from a triangle list
void draw_lines (trianglelist& points, picture& p, const color& c) {
	for (int i = 0; i < points.cols; i += 3) {
		double x1 = points[0][i]  , y1 = points[1][i],
			   x2 = points[0][i+1], y2 = points[1][i+1],
			   x3 = points[0][i+2], y3 = points[1][i+2];

		double dx1 = x2 - x1, dx2 = x3 - x1,
			   dy1 = y2 - y1, dy2 = y3 - y1;

		if (dx1 * dy2 >= dx2 * dy1) {
			draw_line (x1, y1, x2, y2, p, c);
			draw_line (x2, y2, x3, y3, p, c);
			draw_line (x3, y3, x1, y1, p, c);
		}
	}
}

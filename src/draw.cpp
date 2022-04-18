#include <algorithm>
#include <iostream>
#include <cassert>

#include "matrix.hpp"
#include "screen.hpp"
#include "draw.hpp"
#include "color_constants.hpp"


// Draws a line from (x0, y0) to (x1, y1)
void draw_line (int x0, int y0, double z0, int x1, int y1, double z1, picture& p, const color& c, std::vector<std::vector<double>>& zbuffer) {
	// starting point is always left of the ending point
	if (x0 > x1) {
		std::swap (x0, x1);
		std::swap (y0, y1);
		std::swap (z0, z1);
	}

	int x = x0;
	int y = y0;
	double z = z0;
	int A = y1 - y0;
	int B = x0 - x1; 

	if (y1 >= y0 && A <= B * -1) {
		// octant 1
		int D = 2 * A + B;
		double dz = (z0 - z1) / B;

		while (x <= x1) {
			if (0 <= y && y < p.height && 0 <= x && x < p.width && z > zbuffer[y][x]) {
				p[y][x].set(c);
				zbuffer[y][x] = z;
			}
			if (D > 0) {
				D += 2 * B;
				++y;
			}

			++x;
			z += dz;

			D += 2 * A;
		}
	} else if (y1 >= y0) {
		// octant 2
		int D = A + 2 * B;
		double dz = (z1 - z0) / A;

		while (y <= y1) {
			if (0 <= y && y < p.height && 0 <= x && x < p.width && z > zbuffer[y][x]) {
				p[y][x].set(c);
				zbuffer[y][x] = z;
			}
			if (D < 0) {
				D += 2 * A;
				++x;
			}

			++y;
			z += dz;

			D += 2 * B;
		}
	} else if (A >= B) {
		// octant 8
		int D = 2 * A - B;
		double dz = (z0 - z1) / B;

		while (x <= x1) {
			if (0 <= y && y < p.height && 0 <= x && x < p.width && z > zbuffer[y][x]) {
				p[y][x].set(c);
				zbuffer[y][x] = z;
			}
			if (D < 0) {
				D -= 2 * B;
				--y;
			}

			++x;
			z += dz;

			D += 2 * A;
		}
	} else {
		// octant 7
		int D = A - 2 * B;
		double dz = (z1 - z0) / A;

		while (y >= y1) {
			if (0 <= y && y < p.height && 0 <= x && x < p.width && z > zbuffer[y][x]) {
				p[y][x].set(c);
				zbuffer[y][x] = z;
			}
			if (D > 0) {
				D += 2 * A;
				++x;
			}

			--y;
			z += dz;

			D -= 2 * B;
		}
	}
}

// Draws lines from an edge list
void draw_lines (edgelist& points, picture& p, const color& c, std::vector<std::vector<double>>& zbuffer) {
	for (int i = 0; i < points.cols; i += 2) {
		draw_line (points[0][i], points[1][i], points[2][i], points[0][i+1], points[1][i+1], points[2][i+1], p, c, zbuffer);
	}
}

const color col_cyc[] = {RED, LIME, BLUE, YELLOW, CYAN, MAGENTA, NAVY, ORANGE};

// Draws lines from a triangle list
void draw_lines (trianglelist& points, picture& p, const color& c, std::vector<std::vector<double>>& zbuffer) {
	for (int i = 0; i < points.cols; i += 3) {
		double x1 = points[0][i]  , y1 = points[1][i]  , z1 = points[2][i]  ,
			   x2 = points[0][i+1], y2 = points[1][i+1], z2 = points[2][i+1],
			   x3 = points[0][i+2], y3 = points[1][i+2], z3 = points[2][i+2];

		double dx1 = x2 - x1, dx2 = x3 - x1,
			   dy1 = y2 - y1, dy2 = y3 - y1;

		if (dx1 * dy2 > dx2 * dy1 + 1) {
			
			// scanline conversion
			if (y1 < y2) {
				std::swap (y1, y2);
				std::swap (x1, x2);
				std::swap (z1, z2);
			}
			if (y1 < y3) {
				std::swap (y1, y3);
				std::swap (x1, x3);
				std::swap (z1, z3);
			}
			if (y2 < y3) {
				std::swap (y2, y3);
				std::swap (x2, x3);
				std::swap (z2, z3);
			} // values are sorted in order y1, y2, y3
			
			double bx0 = x3; // goes along BT
			double bx1 = ((int)y2 == (int)y3 ? x2 : x3); // bx1 goes along the other 2 edges
			double bz0 = z3;
			double bz1 = ((int)y2 == (int)y3 ? z2 : z3);
			int y = y3;

			double slopeBT = (x1 - x3) / (y1 - y3);
			double slopeBM = (x2 - x3) / (y2 - y3);
			double slopeMT = (x1 - x2) / (y1 - y2);

			double slopeBTz = (z1 - z3) / (y1 - y3);
			double slopeBMz = (z2 - z3) / (y2 - y3);
			double slopeMTz = (z1 - z2) / (y1 - y2);

			while (y <= y1) {
				if(!(bx0 >= std::min({x1, x2, x3}) && bx0 <= std::max({x1, x2, x3}))) {
					++y;
					continue;
				} // hacky, fix later

				draw_line (bx0, y, bz0, bx1, y, bz1, p, col_cyc[i % 8], zbuffer);
				
				// move the endpoints
				bx0 += slopeBT;
				bz0 += slopeBTz;

				if (y < y2 - 1){
					bx1 += slopeBM;
					bz1 += slopeBMz;
				} else {
					bx1 += slopeMT;
					bz1 += slopeMTz;
				}

				++y;

				if (y == (int)y2) {
					bx1 = x2;
					bz1 = z2;
				}
			}

			// draw triangle boundaries
			// draw_line (x1, y1, x2, y2, p, c);
			// draw_line (x2, y2, x3, y3, p, c);
			// draw_line (x3, y3, x1, y1, p, c);
		}
	}
}

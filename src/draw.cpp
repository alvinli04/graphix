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
		double dz = B != 0 ? (z0 - z1) / (B + 1) : 0;

		while (x <= x1) {
			if (0 <= y && y < p.height && 0 <= x && x < p.width && (int)(z * 1000) / 1000.0 > zbuffer[y][x]) {
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
		double dz = A != 0 ? (z1 - z0) / (A + 1) : 0;

		while (y <= y1) {
			if (0 <= y && y < p.height && 0 <= x && x < p.width && (int)(z * 1000) / 1000.0 > zbuffer[y][x]) {
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
		double dz = B != 0 ? (z0 - z1) / (B + 1) : 0;

		while (x <= x1) {
			if (0 <= y && y < p.height && 0 <= x && x < p.width && (int)(z * 1000) / 1000.0 > zbuffer[y][x]) {
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
		double dz = A != 0 ? (z1 - z0) / (A + 1) : 0;

		while (y >= y1) {
			if (0 <= y && y < p.height && 0 <= x && x < p.width && (int)(z * 1000) / 1000.0 > zbuffer[y][x]) {
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

// Get color
color get_color (double x1, double y1, double z1,
				 double x2, double y2, double z2,
				 double x3, double y3, double z3,
				 const color& ambient, std::vector<light>& lights,
				 double ka_r, double kd_r, double ks_r,
				 double ka_g, double kd_g, double ks_g,
				 double ka_b, double kd_b, double ks_b) {
	// Get the normal
	double ax = x2 - x1, ay = y2 - y1, az = z2 - z1;
	double bx = x3 - x1, by = y3 - y1, bz = z3 - z1;
	double Nx = ay * bz - az * by;
	double Ny = az * bx - ax * bz;
	double Nz = ax * by - ay * bx;
	double Nnorm = std::sqrt (Nx * Nx + Ny * Ny + Nz * Nz);
	// normalize
	Nx /= Nnorm, Ny /= Nnorm, Nz /= Nnorm;

	// ambient light
	color c_f (ka_r * ambient.red, ka_g * ambient.green, ka_b * ambient.blue);

	for (light& l : lights) {
		// get L vector
		double lx = l.x;
		double ly = l.y;
		double lz = l.z;
		double lnorm = std::sqrt (l.x * l.x + l.y * l.y + l.z * l.z);
		lx /= lnorm, ly /= lnorm, lz /= lnorm;

		// diffuse
		double cct = lx * Nx + ly * Ny + lz * Nz;
		double ct = std::max (0.0, cct);

		double cdr = kd_r * ct * l.c.red;
		double cdg = kd_g * ct * l.c.green;
		double cdb = kd_b * ct * l.c.blue;

		c_f.red   = (cdr < 255 - c_f.red   ? c_f.red   + cdr : 255);
		c_f.green = (cdg < 255 - c_f.green ? c_f.green + cdg : 255);
		c_f.blue  = (cdb < 255 - c_f.blue  ? c_f.blue  + cdb : 255);

		// specular
		double rfz = std::max (0.0, 2 * ct * Nz - lz);
		rfz = rfz * rfz * rfz;

		cdr = ks_r * rfz * l.c.red;
		cdg = ks_g * rfz * l.c.green;
		cdb = ks_b * rfz * l.c.blue;

		c_f.red   = (cdr < 255 - c_f.red   ? c_f.red   + cdr : 255);
		c_f.green = (cdg < 255 - c_f.green ? c_f.green + cdg : 255);
		c_f.blue  = (cdb < 255 - c_f.blue  ? c_f.blue  + cdb : 255);
	}

	return c_f;
}

// Draws lines from a triangle list
void draw_lines (trianglelist& points, picture& p, const color& c, std::vector<std::vector<double>>& zbuffer,
				 const color& ambient, std::vector<light>& lights,
				 double ka_r, double kd_r, double ks_r,
				 double ka_g, double kd_g, double ks_g,
				 double ka_b, double kd_b, double ks_b) {
	for (int i = 0; i < points.cols; i += 3) {
		double x1 = points[0][i]  , y1 = points[1][i]  , z1 = points[2][i]  ,
			   x2 = points[0][i+1], y2 = points[1][i+1], z2 = points[2][i+1],
			   x3 = points[0][i+2], y3 = points[1][i+2], z3 = points[2][i+2];

		double dx1 = x2 - x1, dx2 = x3 - x1,
			   dy1 = y2 - y1, dy2 = y3 - y1;

		if (dx1 * dy2 > dx2 * dy1) {

			// color calculation
			color c_f = get_color (x1, y1, z1, x2, y2, z2, x3, y3, z3, ambient, lights, ka_r, kd_r, ks_r, ka_g, kd_g, ks_g,ka_b, kd_b, ks_b);

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
			double bx1 = x3; // bx1 goes along the other 2 edges
			double bz0 = z3;
			double bz1 = z3;
			int y = y3;

			double slopeBT = y1 - y3 + 1 > 0 ? (x1 - x3) / (y1 - y3 + 1) : 0;
			double slopeBM = y2 - y3 + 1 > 0 ? (x2 - x3) / (y2 - y3 + 1) : 0;
			double slopeMT = y1 - y2 + 1 > 0 ? (x1 - x2) / (y1 - y2 + 1) : 0;

			double slopeBTz = y1 - y3 + 1 > 0 ? (z1 - z3) / (y1 - y3 + 1) : 0;
			double slopeBMz = y2 - y3 + 1 > 0 ? (z2 - z3) / (y2 - y3 + 1) : 0;
			double slopeMTz = y1 - y2 + 1 > 0 ? (z1 - z2) / (y1 - y2 + 1) : 0;

			bool flip = 0;
			double dx0 = slopeBT;
			double dx1 = slopeBM;
			double dz0 = slopeBTz;
			double dz1 = slopeBMz;


			while (y <= (int)y1) {
				if (!flip && y >= (int)y2) {
					flip = 1;
					dx1 = slopeMT;
					dz1 = slopeMTz;
					bx1 = x2;
					bz1 = z2;
				}

				draw_line (bx0, y, bz0, bx1, y, bz1, p, c_f, zbuffer);
				bx0 += dx0;
				bx1 += dx1;
				bz0 += dz0;
				bz1 += dz1;
				++y;
			}

			// draw triangle boundaries
			// draw_line (x1, y1, z1, x2, y2, z2, p, CYAN, zbuffer);
			// draw_line (x2, y2, z2, x3, y3, z3, p, CYAN, zbuffer);
			// draw_line (x3, y3, z3, x1, y1, z1, p, CYAN, zbuffer);
		}
	}
}

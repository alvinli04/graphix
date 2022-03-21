#include <cmath>
#include <iostream>
#include <vector>

#include "matrix.hpp"
#include "parametric.hpp"

const int density = 20;
const int curve_density = 500;

// draws a circle centered at (cx, cy, cz)
void circle (double cx, double cy, double cz, double r, edgelist& E){
	auto x = [=] (double t) {
		return cx + r * std::cos (2 * t * PI);
	};
	auto y = [=] (double t) {
		return cy + r * std::sin (2 * t * PI);
	};
	double rx = -1, ry;
	for (int t = 0; t <= curve_density; ++t) {
		double currx = x((double) t / curve_density);
		double curry = y((double) t / curve_density);
		if (rx != -1) E.add_edge (rx, ry, cz, currx, curry, cz);
		rx = currx;
		ry = curry;
	}
}

// draws a hermite curve
void hermite (double x0, double y0, double x1, double y1, double rx0, double ry0, double rx1, double ry1, edgelist& E) {
	matrix x_coeffs = H * matrix (
		std::vector<std::vector<double>>
		{
			{x0},
			{x1},
			{rx0},
			{rx1}
		}
	);

	matrix y_coeffs = H * matrix (
		std::vector<std::vector<double>>
		{
			{y0},
			{y1},
			{ry0},
			{ry1}
		}
	);

	auto x = [&] (double t) {
		double a = x_coeffs[0][0];
		double b = x_coeffs[1][0];
		double c = x_coeffs[2][0];
		double d = x_coeffs[3][0];
		return a * t * t * t + b * t * t + c * t + d;
	};
	auto y = [&] (double t) {
		double a = y_coeffs[0][0];
		double b = y_coeffs[1][0];
		double c = y_coeffs[2][0];
		double d = y_coeffs[3][0];
		return a * t * t * t + b * t * t + c * t + d;
	};
	double rx = -1, ry;
	for (int t = 0; t <= curve_density; ++t) {
		double currx = x((double) t / curve_density);
		double curry = y((double) t / curve_density);
		if (rx != -1) E.add_edge (rx, ry, 0, currx, curry, 0);
		rx = currx;
		ry = curry;
	}
}

// draws a bezier curve
void bezier (double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3, edgelist& E) {
	matrix x_coeffs = B * matrix (
		std::vector<std::vector<double>>
		{
			{x0},
			{x1},
			{x2},
			{x3}
		}
	);

	matrix y_coeffs = B * matrix (
		std::vector<std::vector<double>>
		{
			{y0},
			{y1},
			{y2},
			{y3}
		}
	);

	auto x = [&] (double t) {
		double a = x_coeffs[0][0];
		double b = x_coeffs[1][0];
		double c = x_coeffs[2][0];
		double d = x_coeffs[3][0];
		return a * t * t * t + b * t * t + c * t + d;
	};
	auto y = [&] (double t) {
		double a = y_coeffs[0][0];
		double b = y_coeffs[1][0];
		double c = y_coeffs[2][0];
		double d = y_coeffs[3][0];
		return a * t * t * t + b * t * t + c * t + d;
	};
	double rx = -1, ry;
	for (int t = 0; t <= curve_density; ++t) {
		double currx = x((double) t / curve_density);
		double curry = y((double) t / curve_density);
		if (rx != -1) E.add_edge (rx, ry, 0, currx, curry, 0);
		rx = currx;
		ry = curry;
	}
}

// draws a box
void box (double x, double y, double z, double width, double height, double depth, edgelist& E) {
	E.add_edge (x, y, z, x, y, z + depth);
	E.add_edge (x, y, z + depth, x + width, y, z + depth);
	E.add_edge (x + width, y, z + depth, x + width, y, z);
	E.add_edge (x + width, y, z, x, y, z);

	E.add_edge (x, y, z, x, y - height, z);
	E.add_edge (x, y, z + depth, x, y - height, z + depth);
	E.add_edge (x + width, y, z + depth, x + width, y - height, z + depth);
	E.add_edge (x + width, y, z, x + width, y - height, z);

	E.add_edge (x, y - height, z, x, y - height, z + depth);
	E.add_edge (x, y - height, z + depth, x + width, y - height, z + depth);
	E.add_edge (x + width, y - height, z + depth, x + width, y - height, z);
	E.add_edge (x + width, y - height, z, x, y - height, z);
}

// draws a sphere
void sphere (double x, double y, double z, double r, edgelist& E) {
	// generate points
	std::vector<std::vector<double>> points;
	for (int phi = 0; phi < density; ++phi) {
		for (int theta = 0; theta <= density; ++theta) {
			double sx = r * std::cos (PI * theta / density) + x;
			double sy = r * std::sin (PI * theta / density) * std::cos (2 * PI * phi / density) + y;
			double sz = r * std::sin (PI * theta / density) * std::sin (2 * PI * phi / density) + z;
			points.push_back ( {sx, sy, sz} );
		}
	}

	// add points to edge matrix
	for (std::vector<double>& p : points) {
		E.add_edge (p[0], p[1], p[2], p[0], p[1], p[2]);
	}
}

// draws a torus
void torus (double x, double y, double z, double r1, double r2, edgelist& E) {
	// generate points
	std::vector<std::vector<double>> points;
	for (int phi = 0; phi < density; ++phi) {
		for (int theta = 0; theta < density; ++theta) {
			double sx = (r1 * std::cos (2 * PI * theta / density) + r2) * std::cos (2 * PI * phi / density) + x;
			double sy = r1 * std::sin (2 * PI * theta / density) + y;
			double sz = -(r1 * std::cos (2 * PI * theta / density) + r2) * std::sin (2 * PI * phi / density) + z;
			points.push_back ( {sx, sy, sz} );
		}
	}

	// add points to edge matrix
	for (std::vector<double>& p : points) {
		E.add_edge (p[0], p[1], p[2], p[0], p[1], p[2]);
	}
}

#include <cmath>
#include <iostream>
#include <vector>
#include <tuple>

#include "matrix.hpp"
#include "parametric.hpp"

const int density = 21;
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
	for (int t = 0; t <= curve_density - 1; ++t) {
		double currx = x((double) t / (curve_density - 1));
		double curry = y((double) t / (curve_density - 1));
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
	for (int t = 0; t <= curve_density - 1; ++t) {
		double currx = x((double) t / (curve_density - 1));
		double curry = y((double) t / (curve_density - 1));
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
	for (int t = 0; t <= curve_density - 1; ++t) {
		double currx = x((double) t / (curve_density - 1));
		double curry = y((double) t / (curve_density - 1));
		if (rx != -1) E.add_edge (rx, ry, 0, currx, curry, 0);
		rx = currx;
		ry = curry;
	}
}

// draws a box
void box (double x, double y, double z, double width, double height, double depth, trianglelist& T) {
	const auto [x0, y0, z0] = std::make_tuple (x, y, z);
    const auto [x1, y1, z1] = std::make_tuple (x + width, y, z);
    const auto [x2, y2, z2] = std::make_tuple (x, y - height, z);
    const auto [x3, y3, z3] = std::make_tuple (x, y, z - depth);
    const auto [x4, y4, z4] = std::make_tuple (x + width, y - height, z);
    const auto [x5, y5, z5] = std::make_tuple (x + width, y, z - depth);
    const auto [x6, y6, z6] = std::make_tuple (x, y - height, z - depth);
    const auto [x7, y7, z7] = std::make_tuple (x + width, y - height, z - depth);

    // Adding triangles to matrix
    T.add_triangle (x0, y0, z0, x2, y2, z2, x4, y4, z4);
    T.add_triangle (x0, y0, z0, x4, y4, z4, x1, y1, z1);
    T.add_triangle (x3, y3, z3, x6, y6, z6, x2, y2, z2);
    T.add_triangle (x3, y3, z3, x2, y2, z2, x0, y0, z0);
    T.add_triangle (x5, y5, z5, x7, y7, z7, x6, y6, z6);
    T.add_triangle (x5, y5, z5, x6, y6, z6, x3, y3, z3);
    T.add_triangle (x1, y1, z1, x4, y4, z4, x7, y7, z7);
    T.add_triangle (x1, y1, z1, x7, y7, z7, x5, y5, z5);
    T.add_triangle (x3, y3, z3, x0, y0, z0, x1, y1, z1);
    T.add_triangle (x3, y3, z3, x1, y1, z1, x5, y5, z5);
    T.add_triangle (x2, y2, z2, x6, y6, z6, x7, y7, z7);
    T.add_triangle (x2, y2, z2, x7, y7, z7, x4, y4, z4);
}

// draws a sphere
void sphere (double x, double y, double z, double r, trianglelist& T) {
	// generate points
	std::vector<std::vector<double>> points;
	for (int phi = 0; phi < density - 1; ++phi) {
		for (int theta = 0; theta <= density - 1; ++theta) {
			double sx = r * std::cos (PI * theta / (density - 1)) + x;
			double sy = r * std::sin (PI * theta / (density - 1)) * std::cos (2 * PI * phi / (density - 1)) + y;
			double sz = r * std::sin (PI * theta / (density - 1)) * std::sin (2 * PI * phi / (density - 1)) + z;
			points.push_back ( {sx, sy, sz} );
		}
	}

	int sz = points.size();
	// add points to triangle matrix
	for (int i = 0; i < density - 1; i++) { //each semicircle
		for (int j = 0; j < density - 1; j++) { // each point on the semicircle
			T.add_triangle (
				// n, n + 1, n + k + 1
				points[i * density + j][0], points[i * density + j][1], points[i * density + j][2],
				points[i * density + j + 1][0], points[i * density + j + 1][1], points[i * density + j + 1][2],
				points[((i+1) * density + j + 1) % sz][0], points[((i+1) * density + j + 1) % sz][1], points[((i+1) * density + j + 1) % sz][2]
			);

			T.add_triangle (
				//n, n + k + 1, n + k
				points[i * density + j][0], points[i * density + j][1], points[i * density + j][2],
				points[((i+1) * density + j + 1) % sz][0], points[((i+1) * density + j + 1) % sz][1], points[((i+1) * density + j + 1) % sz][2],
				points[((i+1) * density + j) % sz][0], points[((i+1) * density + j) % sz][1], points[((i+1) * density + j) % sz][2]
			);
		}
	}
}

// draws a torus
void torus (double x, double y, double z, double r1, double r2, trianglelist& T) {
	// generate points
	std::vector<std::vector<double>> points;
	for (int phi = 0; phi < density; ++phi) {
		for (int theta = 0; theta < density; ++theta) {
			double sx = (r1 * std::cos (2 * PI * theta / (density - 1)) + r2) * std::cos (2 * PI * phi / (density - 1)) + x;
			double sy = r1 * std::sin (2 * PI * theta / (density - 1)) + y;
			double sz = -(r1 * std::cos (2 * PI * theta / (density - 1)) + r2) * std::sin (2 * PI * phi / (density - 1)) + z;
			points.push_back ( {sx, sy, sz} );
		}
	}

	int sz = points.size();
	// add points to triangle matrix
	for (int i = 0; i < density - 1; i++) { //each circle
		for (int j = 0; j < density - 1; j++) { // each point on the circle
			T.add_triangle (
				// n, n + k + 1, n + 1
				points[i * density + j][0], points[i * density + j][1], points[i * density + j][2],
				points[((i+1) * density + j + 1) % sz][0], points[((i+1) * density + j + 1) % sz][1], points[((i+1) * density + j + 1) % sz][2],
				points[i * density + j + 1][0], points[i * density + j + 1][1], points[i * density + j + 1][2]
			);

			T.add_triangle (
				//n, n + k, n + k + 1
				points[i * density + j][0], points[i * density + j][1], points[i * density + j][2],
				points[((i+1) * density + j) % sz][0], points[((i+1) * density + j) % sz][1], points[((i+1) * density + j) % sz][2],
				points[((i+1) * density + j + 1) % sz][0], points[((i+1) * density + j + 1) % sz][1], points[((i+1) * density + j + 1) % sz][2]
			);
		}
	}
}

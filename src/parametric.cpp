#include <cmath>
#include <iostream>

#include "matrix.hpp"
#include "parametric.hpp"

// draws a circle centered at (cx, cy, cz)
void circle (double cx, double cy, double cz, double r, edgelist& E){
	auto x = [=] (double t) {
		return cx + r * std::cos (2 * t * PI);	
	};
	auto y = [=] (double t) {
		return cy + r * std::sin (2 * t * PI);
	};
	double rx = -1, ry;
	for (double t = 0; t <= 1; t += dt) {
		double currx = x(t);
		double curry = y(t);
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
	for (double t = 0; t <= 1; t += dt) {
		double currx = x(t);
		double curry = y(t);
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
	for (double t = 0; t <= 1; t += dt) {
		double currx = x(t);
		double curry = y(t);
		if (rx != -1) E.add_edge (rx, ry, 0, currx, curry, 0);
		rx = currx;
		ry = curry;
	}
}

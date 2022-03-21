#include "matrix.hpp"

const matrix H (
	std::vector<std::vector<double>>
	{
		{2, -2, 1, 1},
		{-3, 3, -2, -1},
		{0, 0, 1, 0},
		{1, 0, 0 ,0}
	}
);

const matrix B  (
	std::vector<std::vector<double>>
	{
		{-1, 3, -3, 1},
    	{3, -6, 3, 0},
    	{-3, 3, 0, 0},
    	{1, 0, 0, 0}
	}
);

void circle (double cx, double cy, double cz, double r, edgelist& E);
void hermite (double x0, double y0, double x1, double y1, double rx0, double ry0, double rx1, double ry1, edgelist& E);
void bezier (double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3, edgelist& E);
void box (double x, double y, double z, double width, double height, double depth, edgelist& E);
void sphere (double x, double y, double z, double r, edgelist& E);
void torus (double x, double y, double z, double r1, double r2, edgelist& E);

#pragma once

#include <vector>
#include <ostream>
#include <math.h>

const double PI = acos ((double) -1);

class matrix {

public:
	int rows, cols;
	std::vector<std::vector<double>> V;

	matrix ();
	matrix (int rows, int cols);
	matrix (std::vector<std::vector<double>> v);

	void clear ();

	std::vector<double>& operator[](int index);
	friend std::ostream& operator<<(std::ostream& os, matrix const& m);
	matrix& operator*=(const matrix& m);
	friend matrix operator*(matrix a, const matrix& b);

};

void ident (matrix& m);
matrix move (double x, double y, double z);
matrix scale (double x, double y, double z);
matrix rot_x (double theta);
matrix rot_y (double theta);
matrix rot_z (double theta);
//void rot (matrix& m, double theta);

class edgelist: public matrix {

public:
	void add_point (double x, double y, double z);
	void add_edge (double x0, double y0, double z0, double x1, double y1, double z1);
	//void translate (double x, double y, double z = 0);

	edgelist& operator*=(const matrix& m);
	void clear ();
};

class trianglelist: public matrix {

public:
	void add_point (double x, double y, double z);
	void add_triangle (double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2);

	trianglelist& operator*=(const matrix& m);
	void clear ();
};

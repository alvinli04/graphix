#pragma once

#include <vector>
#include <ostream>

const double PI = 3.1415926535;

class matrix {

public:
	int rows, cols;
	std::vector<std::vector<double>> V;

	matrix ();
	matrix (int rows, int cols);

	void clear ();

	std::vector<double>& operator[](int index);
	friend std::ostream& operator<<(std::ostream& os, matrix const& m);
	matrix& operator*=(const matrix& m);
	friend matrix operator*(matrix a, const matrix& b);

};

void ident (matrix& m);
matrix move (double tx, double ty, double tz);
matrix scale (double sx, double sy, double sz);
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

};

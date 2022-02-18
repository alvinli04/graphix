#pragma once

#include <vector>
#include <ostream>

class matrix {

public:
	int rows, cols;
	std::vector<std::vector<double>> V;

	matrix ();
	matrix (int rows, int cols);

	std::vector<double>& operator[](int index);
	friend std::ostream& operator<<(std::ostream& os, matrix& m);
	matrix& operator*(matrix& n, matrix& m);

};

void ident (matrix& m);



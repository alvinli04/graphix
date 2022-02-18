#include <vector>
#include <ostream>
#include <fstream>
#include <iostream>
#include <cassert>

#include "matrix.hpp"

matrix::matrix () {
	rows = 4;
	V.resize (4);
}

matrix::matrix (int rows, int cols) {
	this->rows = rows;
	this->cols = cols;
	V.resize (rows, std::vector<double> (cols));
}

std::vector<double>& matrix::operator[](int index) {
	assert (index < V.size());
	return V[index];
}

std::ostream& operator<<(std::ostream& os, matrix& m) {
	for (int i = 0; i < m.rows; ++i) {
		for (int j = 0; j < m.cols; ++j) {
			os << m[i][j] << " ";
		}
		os << "\n";
	}
	os << "\n";
	return os;
}

matrix& operator*(matrix&n, matrix& m) {
	assert (n.cols == m.rows);
	
	matrix res (n.rows, m.cols);
	for (int i = 0; i < n.rows; ++i) {
		for (int j = 0; j < m.cols; ++j) {
			double sum = 0;
			for (int k = 0; k < n.cols; ++k) {
				sum += n[i][k] * m[k][j];
			}
			res[i][j] = sum;
		}
	}
	return res;
}

int main () {
	matrix m(2,3), n(3,2);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			m[i][j] = 3*i + j + 1;
		}
	}
	n.V = {
		{10, 11},
		{20, 21},
		{30, 31}
	};
	std::cout << m * n;
}











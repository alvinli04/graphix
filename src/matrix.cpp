#include <vector>
#include <ostream>
#include <fstream>
#include <iostream>
#include <cassert>
#include <cmath>

#include "matrix.hpp"

// matrix functions
// -----------------------------------

// Constructors
matrix::matrix () {
	rows = 4;
	cols = 0;
	V.resize (4);
}

matrix::matrix (int rows, int cols) {
	this->rows = rows;
	this->cols = cols;
	V.resize (rows, std::vector<double> (cols));
}

//Clear
void matrix::clear () {
	V = std::vector<std::vector<double>> (rows, std::vector<double> (cols, 0));
}

// Accessor
std::vector<double>& matrix::operator[](int index) {
	assert (index < V.size());
	return V[index];
}

// Printing to output stream
std::ostream& operator<<(std::ostream& os, matrix const& m) {
	for (int i = 0; i < m.rows; ++i) {
		for (int j = 0; j < m.cols; ++j) {
			os << m.V[i][j] << " ";
		}
		os << "\n";
	}
	os << "\n";
	return os;
}

// Matrix Multiplication
matrix& matrix::operator*=(const matrix& m) {
	assert (cols == m.rows);
	
	matrix res (rows, m.cols);
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < m.cols; ++j) {
			double sum = 0;
			for (int k = 0; k < cols; ++k) {
				sum += V[i][k] * m.V[k][j];
			}
			res[i][j] = sum;
		}
	}

	this->V = res.V;
	this->rows = res.rows;
	this->cols = res.cols;
	return *this;
}

matrix operator*(matrix a, const matrix& b) {
	return a *= b;
}

// Turns a matrix into the identity matrix
void ident (matrix& m) {
	assert (m.rows == m.cols);

	m.clear();

	for (int i = 0; i < m.rows; ++i) {
		m[i][i] = 1;
	}
}

// Turns a matrix into a rotation matrix
void rot (matrix&m, double theta) {	
	assert (m.rows == m.cols);

	ident (m);

	m[0][0] = std::cos (theta);
	m[0][1] = std::sin (theta) * -1;
	m[1][0] = std::sin (theta);
	m[1][1] = std::cos (theta);
}


// Edge list functions
// -----------------------------------------------

// Adds one point
void edgelist::add_point (double x, double y, double z) {
	++cols;
	V[0].push_back (x);
	V[1].push_back (y);
	V[2].push_back (z);
	V[3].push_back (1);
}

// Adds an edge
void edgelist::add_edge (double x0, double y0, double z0, double x1, double y1, double z1) {
	add_point (x0, y0, z0);
	add_point (x1, y1, z1);
}

// Set equal to a matrix
edgelist& edgelist::operator*=(const matrix& m) {
	matrix res = m * (*this);
	this->V = res.V;
	this->rows = res.rows;
	this->cols = res.cols;
	return *this;
}

void edgelist::translate (double x, double y, double z) {
	for (double& a : V[0]) {
		a += x;
	}
	for (double& a : V[1]) {
		a += y;
	}
	for (double& a : V[2]) {
		a += z;
	}
}








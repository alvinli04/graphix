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

matrix::matrix (std::vector<std::vector<double>> v) {
	this->rows = v.size();
	this->cols = v[0].size();
	V = v;
}

//Clear
void matrix::clear () {
	V = std::vector<std::vector<double>> (rows, std::vector<double> (cols, 0));
}

// Accessor
std::vector<double>& matrix::operator[](size_t index) {
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

// translation matrix
matrix move (double x, double y, double z) {
	matrix m (4,4);
	ident (m);
	m[0][3] = x;
	m[1][3] = y;
	m[2][3] = z;
	return m;
}

// scale matrix
matrix scale (double x, double y, double z) {
	matrix m (4,4);
	ident (m);
	m[0][0] = x;
	m[1][1] = y;
	m[2][2] = z;
	return m;
}

// x rotation matrix
matrix rot_x (double theta) {
	matrix m (4,4);
	ident (m);
	double angle = theta * PI / 180;
	m[1][1] = std::cos (angle);
	m[1][2] = -std::sin (angle);
	m[2][1] = std::sin (angle);
	m[2][2] = std::cos (angle);
	return m;
}

// y rotation matrix
matrix rot_y (double theta) {
	matrix m (4,4);
	ident (m);
	double angle = theta * PI / 180;
	m[0][0] = std::cos (angle);
	m[0][2] = std::sin (angle);
	m[2][0] = -std::sin (angle);
	m[2][2] = std::cos (angle);
	return m;
}

// z rotation matrix
matrix rot_z (double theta) {
	matrix m (4,4);
	ident (m);
	double angle = theta * PI / 180;
	m[0][0] = std::cos (angle);
	m[0][1] = -std::sin (angle);
	m[1][0] = std::sin (angle);
	m[1][1] = std::cos (angle);
	return m;
}

// Turns a matrix into a rotation matrix (Deprecated)
/*
void rot (matrix&m, double theta) {
	assert (m.rows == m.cols);

	ident (m);

	m[0][0] = std::cos (theta);
	m[0][1] = std::sin (theta) * -1;
	m[1][0] = std::sin (theta);
	m[1][1] = std::cos (theta);
}
*/


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

// Clears the edgelist
void edgelist::clear () {
	V = std::vector<std::vector<double>> (4);
	this->rows = 4;
	this->cols = 0;
}

/* Deprecated translation function
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
*/

// Adds one point
void trianglelist::add_point (double x, double y, double z) {
	++cols;
	V[0].push_back (x);
	V[1].push_back (y);
	V[2].push_back (z);
	V[3].push_back (1);
}

// Adds a triangle
void trianglelist::add_triangle (double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2) {
	add_point (x0, y0, z0);
	add_point (x1, y1, z1);
	add_point (x2, y2, z2);

    point p0 = point { x0, y0, z0 };
    point p1 = point { x1, y1, z1 };
    point p2 = point { x2, y2, z2 };

    double ax = x0 - x1, ay = y0 - y1, az = z0 - z1;
    double bx = x0 - x2, by = y0 - y2, bz = z0 - z2;
    point norm = { 0, 0, 0 };
    norm.x += ay * bz - az * by;
    norm.y += az * bx - ax * bz;
    norm.z += ax * by - ay * bx;

    double mag = sqrt(norm.x * norm.x + norm.y * norm.y + norm.z * norm.z);
    norm.x /= mag, norm.y /= mag, norm.z /= mag;

    for (auto p : {p0, p1, p2}) {
        vertex_normals[p].x += norm.x;
        vertex_normals[p].y += norm.y;
        vertex_normals[p].z += norm.z;
    }


    // std::cerr << "new thing!!!" << std::endl;
    // for (auto i = vertex_normals.begin(); i != vertex_normals.end(); i++) {
    //     std::cerr << "thing: " << i->first.x << " " << i->first.y << " " << i->first.z << std::endl;
    // }

}

point trianglelist::get_vertex_normal(double px, double py, double pz) {
    point norm = vertex_normals[point { px, py, pz} ];
    double mag = sqrt(norm.x * norm.x + norm.y * norm.y + norm.z * norm.z);
    norm.x /= mag, norm.y /= mag, norm.z /= mag;
    return norm;
}

// Multiplication
trianglelist& trianglelist::operator*=(const matrix& m) {
	matrix res = m * (*this);
	this->V = res.V;
	this->rows = res.rows;
	this->cols = res.cols;

	// recalculate normals
	vertex_normals.clear();
	for (int i = 0; i < cols; i += 3) {
		double x0 = V[0][i], x1 = V[0][i+1], x2 = V[0][i+2],
			   y0 = V[1][i], y1 = V[1][i+1], y2 = V[1][i+2],
			   z0 = V[2][i], z1 = V[2][i+1], z2 = V[2][i+2];

		point p0 = point { x0, y0, z0 };
	    point p1 = point { x1, y1, z1 };
	    point p2 = point { x2, y2, z2 };

	    double ax = x0 - x1, ay = y0 - y1, az = z0 - z1;
	    double bx = x0 - x2, by = y0 - y2, bz = z0 - z2;
	    point norm = { 0, 0, 0 };
	    norm.x += ay * bz - az * by;
	    norm.y += az * bx - ax * bz;
	    norm.z += ax * by - ay * bx;

	    double mag = sqrt(norm.x * norm.x + norm.y * norm.y + norm.z * norm.z);
	    norm.x /= mag, norm.y /= mag, norm.z /= mag;

	    for (auto p : {p0, p1, p2}) {
	        vertex_normals[p].x += norm.x;
	        vertex_normals[p].y += norm.y;
	        vertex_normals[p].z += norm.z;
	    }
	}

	return *this;
}

// Clears the trianglelist
void trianglelist::clear () {
	V = std::vector<std::vector<double>> (4);
	this->rows = 4;
	this->cols = 0;
}

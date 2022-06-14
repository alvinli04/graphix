#pragma once

#include <vector>
#include <ostream>
#include <math.h>
#include <unordered_map>

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
	matrix& operator=(const matrix& m);
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

struct point {
    double x, y, z;
};

//for the below I know double are rarely equal but this program is a case where 
//there actually might be a fair few doubles which are super equal
//if there are problems though this is a point to rethink and I will put FIXME
//point hash function
template<>
struct std::hash<point> {
    std::size_t operator()(point const& p) const noexcept {
        //method taken from boost::hash_combine
        double a = p.x, b = p.y ,c = p.z;
        size_t h1 = std::hash<double>{}(a);
        size_t h2 = std::hash<double>{}(b);
        size_t h3 = std::hash<double>{}(c);
        size_t out = h1 + 0x9e3779b9 + (h2 << 6) + (h2 >> 2);
        return out + 0x9e3779b9 + (h3 << 6) + (h3 >> 2);
    }
};
//same goes for this equality function
template<>
struct std::equal_to<point> {
    constexpr bool operator()(const point &p, const point &q) const {
        return p.x == q.x && p.y == q.y && p.z == q.z;
    }
};



class trianglelist: public matrix {

public:
	void add_point (double x, double y, double z);
    // the key is a point and maps to the current unnormalized sum of normalized face vectors
    std::unordered_map<point, point> vertex_normals;
    point get_vertex_normal(double px, double py, double pz);
	void add_triangle (double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2);

	trianglelist& operator*=(const matrix& m);
	void clear ();
};

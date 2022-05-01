#include <cmath>
#include <iostream>
#include <stack>

#include "screen.hpp"
#include "draw.hpp"
#include "color_constants.hpp"
#include "matrix.hpp"
#include "parser.hpp"
#include "parametric.hpp"

const int N = 500;
const double ka = .33;
const double kd = .33;
const double ks = .33;

int main (int argc, char** argv) {

    picture S (N, N);
	edgelist E; // Edgelist that holds all edges
	trianglelist T; // Triangle list for 3D shapes
	std::vector<std::vector<double>> zbuffer (N, std::vector<double> (N, -std::numeric_limits<double>::infinity()));

	matrix M (4,4); // Keeps track of all transformations
    ident (M);
	std::stack<matrix> cstack;
	cstack.push(M);

	color ambient (100, 100, 100);
	std::vector<light> lights;
	lights.push_back (light (500, 500, 500, color (255, 255, 255)));

    if (argc == 2) 
    	parse_file (argv[1], cstack, E, T, S, zbuffer, ambient, lights, ka, kd, ks);
    else 
    	parse_file ("stdin", cstack, E, T, S, zbuffer, ambient, lights, ka, kd, ks);
}

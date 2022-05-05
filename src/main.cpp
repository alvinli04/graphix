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


int main (int argc, char** argv) {

    picture S (N, N);
	edgelist E; // Edgelist that holds all edges
	trianglelist T; // Triangle list for 3D shapes
	std::vector<std::vector<double>> zbuffer (N, std::vector<double> (N, -std::numeric_limits<double>::infinity()));

	matrix M (4,4); // Keeps track of all transformations
    ident (M);
	std::stack<matrix> cstack;
	cstack.push(M);

	std::vector<light> lights; // vector with all point light sources

	// temporary lighting stuff
    // for (int i = 0; i < N; ++i) for (int j = 0; j < N; ++j) S[i][j].set(color(78/4.0, 84/4.0, 129/4.0));
	color ambient (50, 50, 50);
	lights.push_back (light (-.5, 2, 1, color (255, 255, 255)));
    lights.push_back (light (.5, -2, -1, color (0, 0, 100)));

	double ka_r = 0.5, ka_g = 0.5, ka_b = 1,
           kd_r = 1, kd_g = 0.5, kd_b = 0.5,
           ks_r = 0.5, ks_g = 1, ks_b = 0.5;



    if (argc == 2)
    	parse_file (argv[1], cstack, E, T, S, zbuffer, ambient, lights, ka_r, kd_r, ks_r, ka_g, kd_g, ks_g, ka_b, kd_b, ks_b);
    else
    	parse_file ("stdin", cstack, E, T, S, zbuffer, ambient, lights, ka_r, kd_r, ks_r, ka_g, kd_g, ks_g,ka_b, kd_b, ks_b);
}

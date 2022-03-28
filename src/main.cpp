#include <cmath>
#include <iostream>

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
	matrix M (4,4); // Keeps track of all transformations
    ident (M);
	
    if (argc == 2) parse_file (argv[1], M, E, T, S);
    else parse_file ("stdin", M, E, T, S);
}

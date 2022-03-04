#include <cmath>
#include <iostream>

#include "screen.hpp"
#include "draw.hpp"
#include "color_constants.hpp"
#include "matrix.hpp"
#include "parser.hpp"

const int N = 500;

int main () {

    picture S (N, N);
	edgelist E; // Edgelist that holds all edges
	matrix M (4,4); // Keeps track of all transformations

	parse_file ("test.in", M, E, S);


    S.to_ppm ("test");

}

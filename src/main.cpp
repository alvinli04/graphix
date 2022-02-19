#include <cmath>
#include <iostream>

#include "screen.hpp"
#include "draw.hpp"
#include "color_constants.hpp"
#include "matrix.hpp"

#define N 500
#define PI 3.1415926535



int main () {
	
    picture s(N, N);

    edgelist edges;

	edges.add_edge (50, 60, 0, 30, 20, 0);
	edges.add_edge (50, 60, 0, 70, 20, 0);
    edges.add_edge (30, 20, 0, 70, 20, 0);

	draw_lines (edges, s, CYAN);

	matrix D (4, 4);
	

	ident (D);
	D[0][0] = 1.8;
	D[1][1] = 1.8;

	edges *= D;
	draw_lines (edges, s, RED);

	edges *= D;
	draw_lines (edges, s, LIME);

    s.to_ppm("test"); 
	
}

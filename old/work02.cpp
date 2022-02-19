#include <cmath>
#include <iostream>
#include "screen.hpp"
#include "draw.hpp"
#include "color_constants.hpp"

#define N 500
#define PI 3.1415926535



int main () {
	
    picture s(N, N);

    edgelist edges;

	edges.add_edge(50, 450, 0, 100, 450, 0);
	edges.add_edge(50, 450, 0, 50, 400, 0);
	edges.add_edge(100, 450, 0, 100, 400, 0);
	edges.add_edge(100, 400, 0, 50, 400, 0);

	edges.add_edge(200, 450, 0, 250, 450, 0);
	edges.add_edge(200, 450, 0, 200, 400, 0);
	edges.add_edge(250, 450, 0, 250, 400, 0);
	edges.add_edge(250, 400, 0, 200, 400, 0);

	edges.add_edge(150, 400, 0, 130, 360, 0);
	edges.add_edge(150, 400, 0, 170, 360, 0);
	edges.add_edge(130, 360, 0, 170, 360, 0);
	
	edges.add_edge(100, 340, 0, 200, 340, 0);
	edges.add_edge(100, 320, 0, 200, 320, 0);
	edges.add_edge(100, 340, 0, 100, 320, 0);
	edges.add_edge(200, 340, 0, 200, 320, 0);

	draw_lines (edges, s, CYAN);

    s.to_ppm("test"); 
	
}

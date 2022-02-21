#include <cmath>
#include <iostream>
#include "screen.hpp"
#include "draw.hpp"
#include "color_constants.hpp"

#define N 500
#define PI 3.1415926535



int main () {
	
	edgelist m2;
	m2.add_edge (1, 2, 3, 4, 5, 6);
	std::cout << "Testing add_edge. Adding (1, 2, 3), (4, 5, 6) m2 =\n" << m2;

	matrix m1 (4, 4);
	ident (m1);
	std::cout << "Testing ident. m1 =\n" << m1;

	std::cout << "Testing Matrix mult. m1 * m2 =\n" << m1 * m2;
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			m1[i][j] = i + 1 + 3 * j;
		}
	}
	for (int i = 0; i < 4; i++){
		m1[3][i] = 1;
	}
	std::cout << "Testing Matrix mult. m1=\n" << m1;
	std::cout << "Testing Matrix mult. m1 * m2 =\n" << m1 * m2;


	// Generate sample image

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

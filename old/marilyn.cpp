#include <cmath>
#include <iostream>

#include "screen.hpp"
#include "draw.hpp"
#include "color_constants.hpp"
#include "matrix.hpp"

#define N 500
#define PI 3.1415926535


int main () {
	
    picture p (N, N);

    edgelist edges;

	edges.add_edge (189, 353, 0, 273, 376, 0);
	edges.add_edge (273, 376, 0, 325, 368, 0);
	edges.add_edge (325, 368, 0, 339, 303, 0);
	edges.add_edge (339, 303, 0, 343, 243, 0);
	edges.add_edge (343, 243, 0, 330, 93, 0);
	edges.add_edge (330, 93, 0, 300, 50, 0);
	edges.add_edge (300, 50, 0, 254, 32, 0);
	edges.add_edge (254, 32, 0, 191, 17, 0);
	edges.add_edge (191, 17, 0, 152, 30, 0);
	edges.add_edge (152, 30, 0, 116, 66, 0);
	edges.add_edge (116, 66, 0, 103, 138, 0);
	edges.add_edge (103, 138, 0, 90, 192, 0);
	edges.add_edge (90, 192, 0, 95, 276, 0);
	edges.add_edge (95, 276, 0, 131, 310, 0);
	edges.add_edge (131, 310, 0, 121, 344, 0);
	edges.add_edge (121, 344, 0, 139, 369, 0);
	edges.add_edge (139, 369, 0, 189, 353, 0);

	edges.add_edge (222, 231, 0, 250, 250, 0);
	edges.add_edge (250, 250, 0, 292, 235, 0);

	edges.add_edge (170, 231, 0, 138, 245, 0);
	edges.add_edge (138, 245, 0, 114, 230, 0);

	

	edgelist hair;

	hair.add_edge (140, 38, 0, 113, 37, 0);
	hair.add_edge (113, 37, 0, 100, 60, 0);
	hair.add_edge (100, 60, 0, 82, 110, 0);
	hair.add_edge (82, 110, 0, 49, 164, 0);
	hair.add_edge (49, 164, 0, 57, 198, 0);
	hair.add_edge (57, 198, 0, 34, 256, 0);
	hair.add_edge (34, 256, 0, 54, 312, 0);
	hair.add_edge (54, 312, 0, 43, 362, 0);
	hair.add_edge (43, 362, 0, 46, 417, 0);
	hair.add_edge (46, 417, 0, 89, 446, 0);
	hair.add_edge (89, 446, 0, 129, 484, 0);
	hair.add_edge (129, 484, 0, 258, 492, 0);
	hair.add_edge (258, 492, 0, 313, 472, 0);
	hair.add_edge (313, 472, 0, 352, 428, 0);
	hair.add_edge (352, 428, 0, 395, 426, 0);
	hair.add_edge (395, 426, 0, 424, 407, 0);
	hair.add_edge (424, 407, 0, 450, 350, 0);
	hair.add_edge (450, 350, 0, 450, 300, 0);
	hair.add_edge (450, 300, 0, 468, 251, 0);
	hair.add_edge (468, 251, 0, 440, 169, 0);
	hair.add_edge (440, 169, 0, 459, 159, 0);
	hair.add_edge (459, 159, 0, 393, 90, 0);
	hair.add_edge (393, 90, 0, 377, 40, 0);
	hair.add_edge (377, 40, 0, 325, 78, 0);

	hair.add_edge (250, 225, 0, 257, 217, 0);
	hair.add_edge (257, 217, 0, 271, 218, 0);

	

	edgelist features;

	features.add_edge (144, 108, 0, 171, 123, 0);
	features.add_edge (171, 123, 0, 191, 120, 0);
	features.add_edge (191, 120, 0, 216, 124, 0);
	features.add_edge (216, 124, 0, 249, 107, 0);
	features.add_edge (249, 107, 0, 208, 70, 0);
	features.add_edge (208, 70, 0, 167, 71, 0);
	features.add_edge (167, 71, 0, 144, 108, 0);

	features.add_edge (168, 154, 0, 189, 147, 0);
	features.add_edge (189, 147, 0, 223, 153, 0);

	features.add_edge (232, 267, 0, 278, 276, 0);
	features.add_edge (278, 276, 0, 321, 251, 0);

	features.add_edge (158, 263, 0, 122, 273, 0);
	features.add_edge (122, 273, 0, 100, 250, 0);

	features.add_edge (233, 225, 0, 261, 233, 0);
	features.add_edge (261, 233, 0, 298, 223, 0);

	features.add_edge (107, 222, 0, 133, 233, 0);
	features.add_edge (133, 233, 0, 166, 226, 0);

	features.add_edge (152, 228, 0, 149, 220, 0);
	features.add_edge (149, 220, 0, 135, 220, 0);
	


	matrix D (4, 4);
	ident (D);

	D[0][0] = 1.0 / 3;
	D[1][1] = 1.0 / 3;

	edges *= D;
	hair *= D;
	features *= D;

	draw_lines (edges, p, PINK);
	draw_lines (hair, p, YELLOW);
	draw_lines (features, p, PURPLE);

	edges.translate (167, 0);
	hair.translate (167, 0);
	features.translate (167, 0);
	draw_lines (edges, p, RED);
	draw_lines (hair, p, WHITE);
	draw_lines (features, p, YELLOW);

	edges.translate (167, 0);
	hair.translate (167, 0);
	features.translate (167, 0);
	draw_lines (edges, p, TEAL);
	draw_lines (hair, p, PURPLE);
	draw_lines (features, p, GREEN);

	edges.translate (0, 167);
	hair.translate (0, 167);
	features.translate (0, 167);
	draw_lines (edges, p, SILVER);
	draw_lines (hair, p, MAGENTA);
	draw_lines (features, p, CYAN);

	edges.translate (-167, 0);
	hair.translate (-167, 0);
	features.translate (-167, 0);
	draw_lines (edges, p, PINK);
	draw_lines (hair, p, YELLOW);
	draw_lines (features, p, PURPLE);

	edges.translate (-167, 0);
	hair.translate (-167, 0);
	features.translate (-167, 0);
	draw_lines (edges, p, GREEN);
	draw_lines (hair, p, SILVER);
	draw_lines (features, p, MAROON);

	edges.translate (0, 167);
	hair.translate (0, 167);
	features.translate (0, 167);
	draw_lines (edges, p, ORANGE);
	draw_lines (hair, p, LIME);
	draw_lines (features, p, OLIVE);

	edges.translate (167, 0);
	hair.translate (167, 0);
	features.translate (167, 0);
	draw_lines (edges, p, MAROON);
	draw_lines (hair, p, BLUE);
	draw_lines (features, p, LIME);
	

	edges.translate (167, 0);
	hair.translate (167, 0);
	features.translate (167, 0);
	draw_lines (edges, p, PINK);
	draw_lines (hair, p, YELLOW);
	draw_lines (features, p, PURPLE);

    p.to_ppm("test"); 
	
}

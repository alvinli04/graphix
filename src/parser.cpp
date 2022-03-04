#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>

#include "parser.hpp"
#include "matrix.hpp"
#include "draw.hpp"
#include "screen.hpp"
#include "color_constants.hpp"

void parse_file (std::string filename, matrix& M, edgelist& E, picture& S) {
	std::ifstream fin (filename);
	std::cout << "Opened " << filename << "\n";

	S.clear ();
	std::string cmd;
	while (fin >> cmd) {
		if (cmd == "line") {
			double x0, y0, z0, x1, y1, z1;
			fin >> x0 >> y0 >> z0 >> x1 >> y1 >> z1;
			E.add_edge (x0, y0, z0, x1, y1, z1);
		} else if (cmd == "ident") {
			ident (M);
		} else if (cmd == "scale") {
			double x, y, z;
			fin >> x >> y >> z;
			M = scale (x, y, z) * M;
		} else if (cmd == "move") {
			double x, y, z;
			fin >> x >> y >> z;
			M = move (x, y, z) * M;
		} else if (cmd == "rotate") {
			char axis;
			double theta;
			fin >> axis >> theta;
			if (axis == 'x') {
				M = rot_x (theta) * M;
			} else if (axis == 'y') {
				M = rot_y (theta) * M;
			} else if (axis == 'z') {
				M = rot_z (theta) * M;
			}
		} else if (cmd == "apply") {
			E *= M;
		} else if (cmd == "display") {
			S.clear ();
			draw_lines (E, S, WHITE);
			S.to_ppm ("test");
			std::system ("display test.ppm");
			std::system ("rm test.ppm");
		} else if (cmd == "save") {
			std::string fname;
			fin >> fname;
			S.clear ();
			draw_lines (E, S, WHITE);
			S.to_ppm (fname);
		}
		// std::cout << cmd << std::endl;
	}

	fin.close ();
}

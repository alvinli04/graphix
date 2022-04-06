#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stack>

#include "parser.hpp"
#include "matrix.hpp"
#include "draw.hpp"
#include "screen.hpp"
#include "color_constants.hpp"
#include "parametric.hpp"

void parse_file (std::string filename, std::stack<matrix>& cstack, edgelist& E, trianglelist& T, picture& S) {
	std::ifstream fin (filename);
	std::cout << "Opened " << filename << "\n";

	S.clear ();
	std::string cmd;
	while (fin >> cmd) {
		if (cmd[0] == '#') {
			//skip the line
			fin.ignore(256, '\n');
		} else if (cmd == "line") {
			double x0, y0, z0, x1, y1, z1;
			fin >> x0 >> y0 >> z0 >> x1 >> y1 >> z1;
			E.add_edge (x0, y0, z0, x1, y1, z1);
			E *= cstack.top();
			draw_lines (E, S, WHITE);
			E.clear();
			matrix M = cstack.top();
			cstack.push (M);
		} else if (cmd == "ident") {
			ident (cstack.top());
		} else if (cmd == "scale") {
			double x, y, z;
			fin >> x >> y >> z;
			cstack.top() *= scale (x, y, z);
		} else if (cmd == "move") {
			double x, y, z;
			fin >> x >> y >> z;
			cstack.top() *= move (x, y, z);
		} else if (cmd == "rotate") {
			char axis;
			double theta;
			fin >> axis >> theta;
			if (axis == 'x') {
				cstack.top() *= rot_x (theta);
			} else if (axis == 'y') {
				cstack.top() *= rot_y (theta);
			} else if (axis == 'z') {
				cstack.top() *= rot_z (theta);
			}
		} else if (cmd == "circle") {
			double cx, cy, cz, r;
			fin >> cx >> cy >> cz >> r;
			circle (cx, cy, cz, r, E);
			E *= cstack.top();
			draw_lines (E, S, WHITE);
			E.clear();
			matrix M = cstack.top();
			cstack.push (M);
		} else if (cmd == "hermite") {
			double x0, y0, x1, y1, rx0, ry0, rx1, ry1;
			fin >> x0 >> y0 >> x1 >> y1 >> rx0 >> ry0 >> rx1 >> ry1;
			hermite (x0, y0, x1, y1, rx0, ry0, rx1, ry1, E);
			E *= cstack.top();
			draw_lines (E, S, WHITE);
			E.clear();
			matrix M = cstack.top();
			cstack.push (M);
		} else if (cmd == "bezier") {
			double x0, y0, x1, y1, x2, y2, x3, y3;
			fin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
			bezier (x0, y0, x1, y1, x2, y2, x3, y3, E);
			E *= cstack.top();
			draw_lines (E, S, WHITE);
			E.clear();
			matrix M = cstack.top();
			cstack.push (M);
		} else if (cmd == "box") {
			double x, y, z, width, height, depth;
			fin >> x >> y >> z >> width >> height >> depth;
			box (x, y, z, width, height, depth, T);
			T *= cstack.top();
			draw_lines (T, S, WHITE);
			T.clear();
			matrix M = cstack.top();
			cstack.push (M);
		} else if (cmd == "sphere") {
			double x, y, z, r;
			fin >> x >> y >> z >> r;
			sphere (x, y, z, r, T);
			T *= cstack.top();
			draw_lines (T, S, WHITE);
			T.clear();
			matrix M = cstack.top();
			cstack.push (M);
		} else if (cmd == "torus") {
			double x, y, z, r1, r2;
			fin >> x >> y >> z >> r1 >> r2;
			torus (x, y, z, r1, r2, T);
		} else if (cmd == "clear") {
			E.clear();
			T.clear();
		} else if (cmd == "apply") {
			E *= cstack.top();
			T *= cstack.top();
		} else if (cmd == "push") {
			matrix m (4, 4);
			ident (m);
			cstack.push (m);
		} else if (cmd == "pop") {
			cstack.pop();
		} else if (cmd == "display") {
			S.to_ppm ("/tmp/test");
			std::system ("display /tmp/test.ppm");
		} else if (cmd == "save") {
			std::string fname;
			fin >> fname;
			S.clear ();
			draw_lines (E, S, WHITE);
			draw_lines (T, S, WHITE);
			fname = "img/" + fname;
			S.to_ppm (fname);
			std::string fsave = "convert " + fname + ".ppm " + fname;
			std::system (fsave.c_str ());
			std::string fdel = "rm " + fname + ".ppm";
			std::system (fdel.c_str ());
		}
	}

	fin.close ();
}

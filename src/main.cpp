#include <cmath>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <stack>
#include <unordered_map>
#include <vector>
#include <variant>

#include "screen.hpp"
#include "draw.hpp"
#include "color_constants.hpp"
#include "matrix.hpp"
#include "parametric.hpp"

const int N = 500;

using constants = std::array<std::array<double, 3>, 3>;
struct symbol {
    constants c;
};

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
    color ambient (50, 50, 50); // ambient light
    lights.push_back (light (-.5, 2, 1, color (255, 255, 255)));
    lights.push_back (light (.5, -2, -1, color (0, 0, 100)));

    std::unordered_map<std::string, symbol> symtab;

    // parse the symbol table
   	std::ifstream symin ("src/compiler/mdl.sym");
    std::string sym;
    while (symin >> sym) {
        std::string name;
        symin >> name;
        if (sym == "constants") {
            double ka_r, kd_r, ks_r,
                   ka_g, kd_g, ks_g,
                   ka_b, kd_b, ks_b;

            symin >> ka_r >> kd_r >> ks_r;
            symin >> ka_g >> kd_g >> ks_g;
            symin >> ka_b >> kd_b >> ks_b;

            symtab[name].c = {{
                {{ka_r, kd_r, ks_r}},
                {{ka_g, kd_g, ks_g}},
                {{ka_b, kd_b, ks_b}}
            }};
        }
    }

   	// parse the command list
   	std::ifstream cmdin ("src/compiler/mdl.cmd");
    std::string cmd, none;
    while (cmdin >> cmd) {
        if (cmd == "constants") {
			//skip the line
			cmdin.ignore(256, '\n');
		} else if (cmd == "push") {
            cmdin >> none;
            matrix m = cstack.top();
			cstack.push (m);
        } else if (cmd == "pop") {
            cmdin >> none;
            cstack.pop ();
        } else if (cmd == "move") {
            double x, y, z;
			cmdin >> x >> y >> z;
            cmdin >> none;
			cstack.top() *= move (x, y, z);
        } else if (cmd == "rotate") {
            char axis;
            double theta;
            cmdin >> axis >> theta;
            cmdin >> none;
            if (axis == 'x') {
                cstack.top() *= rot_x (theta);
            } else if (axis == 'y') {
                cstack.top() *= rot_y (theta);
            } else if (axis == 'z') {
                cstack.top() *= rot_z (theta);
            }
        } else if (cmd == "scale") {
            double x, y, z;
            cmdin >> x >> y >> z;
            cmdin >> none;
            cstack.top() *= scale (x, y, z);
        } else if (cmd == "box") {
            std::string constants;
            cmdin >> constants;
            cmdin >> none;
            double x, y, z, width, height, depth;
			cmdin >> x >> y >> z >> width >> height >> depth;
			box (x, y, z, width, height, depth, T);
			T *= cstack.top();

            // get lighting
            double ka_r = .5, kd_r = .5, ks_r = .5,
                   ka_g = .5, kd_g = .5, ks_g = .5,
                   ka_b = .5, kd_b = .5, ks_b = .5;

            if (constants != "None") {
                std::array<std::array<double, 3>, 3> curr = symtab[constants].c;
                ka_r = curr[0][0];
                kd_r = curr[0][1];
                ks_r = curr[0][2];
                ka_g = curr[1][0];
                kd_g = curr[1][1];
                ks_g = curr[1][2];
                ka_b = curr[2][0];
                kd_b = curr[2][1];
                ks_b = curr[2][2];
            }

			draw_lines (T, S, WHITE, zbuffer, ambient, lights, ka_r, kd_r, ks_r, ka_g, kd_g, ks_g, ka_b, kd_b, ks_b);
			T.clear();
        } else if (cmd == "sphere") {
            std::string constants;
            cmdin >> constants;
            cmdin >> none;
            double x, y, z, r;
			cmdin >> x >> y >> z >> r;
			sphere (x, y, z, r, T);
			T *= cstack.top();

            // get lighting
            double ka_r = .5, kd_r = .5, ks_r = .5,
                   ka_g = .5, kd_g = .5, ks_g = .5,
                   ka_b = .5, kd_b = .5, ks_b = .5;

            if (constants != "None") {
                std::array<std::array<double, 3>, 3> curr = symtab[constants].c;
                ka_r = curr[0][0];
                kd_r = curr[0][1];
                ks_r = curr[0][2];
                ka_g = curr[1][0];
                kd_g = curr[1][1];
                ks_g = curr[1][2];
                ka_b = curr[2][0];
                kd_b = curr[2][1];
                ks_b = curr[2][2];
            }

			draw_lines (T, S, WHITE, zbuffer, ambient, lights, ka_r, kd_r, ks_r, ka_g, kd_g, ks_g, ka_b, kd_b, ks_b);
			T.clear();
        } else if (cmd == "torus") {
            std::string constants;
            cmdin >> constants;
            cmdin >> none;
            double x, y, z, r1, r2;
			cmdin >> x >> y >> z >> r1 >> r2;
			torus (x, y, z, r1, r2, T);
			T *= cstack.top();

            // get lighting
            double ka_r = .5, kd_r = .5, ks_r = .5,
                   ka_g = .5, kd_g = .5, ks_g = .5,
                   ka_b = .5, kd_b = .5, ks_b = .5;

            if (constants != "None") {
                std::array<std::array<double, 3>, 3> curr = symtab[constants].c;
                ka_r = curr[0][0];
                kd_r = curr[0][1];
                ks_r = curr[0][2];
                ka_g = curr[1][0];
                kd_g = curr[1][1];
                ks_g = curr[1][2];
                ka_b = curr[2][0];
                kd_b = curr[2][1];
                ks_b = curr[2][2];
            }

			draw_lines (T, S, WHITE, zbuffer, ambient, lights, ka_r, kd_r, ks_r, ka_g, kd_g, ks_g,ka_b, kd_b, ks_b);
			T.clear();
        } else if (cmd == "line") {
            cmdin >> none;
            double x0, y0, z0, x1, y1, z1;
			cmdin >> x0 >> y0 >> z0 >> x1 >> y1 >> z1;
			E.add_edge (x0, y0, z0, x1, y1, z1);
			E *= cstack.top();
			draw_lines (E, S, WHITE, zbuffer);
			E.clear();
        } else if (cmd == "save") {
            std::string fname;
			cmdin >> fname;
			fname = "img/" + fname;
			S.to_ppm (fname);
			std::string fsave = "convert " + fname + ".ppm " + fname;
			std::system (fsave.c_str ());
			std::string fdel = "rm " + fname + ".ppm";
			std::system (fdel.c_str ());
        } else if (cmd == "display") {
            cmdin >> none;
            S.to_ppm ("/tmp/test");
			std::system ("display /tmp/test.ppm");
        }
        // std::cout << cmd << '\n';
    }

    symin.close();
   	cmdin.close();

}

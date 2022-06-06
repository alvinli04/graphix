#include <cmath>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <stack>
#include <unordered_map>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <array>
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
    double val;
};

using command = std::variant<double, std::string>;

int main (int argc, char** argv) {
    picture S (N, N);
	edgelist E; // Edgelist that holds all edges
	trianglelist T; // Triangle list for 3D shapes
	std::vector<std::vector<double>> zbuffer (N, std::vector<double> (N, -std::numeric_limits<double>::infinity()));
    int frames = 1;
    std::string basename = "frame";

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
    std::string cmd, none, knob;
    std::vector<command> cmdlist;
    while (cmdin >> cmd) {
        if (std::isdigit(cmd[0]) || cmd[0] == '-'){
            cmdlist.push_back(command(stod(cmd)));
        } else {
            cmdlist.push_back(command(cmd));
        }
    }

    const int num_cmd = cmdlist.size();
    int cnt = 0;

    // pass 0
    bool vary_h = 0, basename_h = 0, frames_h = 0;
    while (cnt < num_cmd) {
        cmd = (cmdlist[cnt].index() == 1 ? std::get<std::string>(cmdlist[cnt]) : "");
        if (cmd == "frames") {
            ++cnt;
            frames = std::get<double>(cmdlist[cnt]);
            frames_h = 1;
        } else if (cmd == "basename") {
            ++cnt;
            basename = std::get<std::string>(cmdlist[cnt]);
            basename_h = 1;
        } else if (cmd == "vary") {
            vary_h = 1;
        }
        ++cnt;
    }

    if (vary_h && !frames_h) {
        std::cout << "Error: vary is present but the number of frames is not defined.\n";
        return 0;
    }
    if (frames_h && !basename_h) {
        std::cout << "Warning: basename is not present. Setting basename to \"frame\".";
    }

    // pass 1
    std::vector<std::map<std::string, double>> frame_list(frames);

    // reset cnt
    cnt = 0;

    while (cnt < num_cmd) {
        cmd = (cmdlist[cnt].index() == 1 ? std::get<std::string>(cmdlist[cnt]) : "");
        if (cmd == "vary") {
            std::string knob_name = std::get<std::string>(cmdlist[cnt + 5]);
            double sframe = std::get<double>(cmdlist[cnt + 1]), eframe = std::get<double>(cmdlist[cnt + 2]);
            double sval = std::get<double>(cmdlist[cnt + 3]), eval = std::get<double>(cmdlist[cnt + 4]);
            // std::printf("%f, %f, %f, %f ", sframe, eframe, sval, eval);
            // std::cout << knob_name << "]\n";
            for (int i = (int)sframe; i <= (int)eframe; i++) {
                frame_list[i][knob_name] = sval + (i - (int)sframe) * (eval - sval) / ((int)eframe - (int)sframe);
            }
            symtab[knob_name].val = sval;
            cnt += 5;
        }
        ++cnt;
    }
    /*
    for(int i = 0; i < frames; i++){
    	std::cout << "frame " << i << "\n";
    	for (auto& p : frame_list[i]) {
    		std::cout << p.first << " " << p.second << "\n";
    	}
    }
    */

    // pass 2

    // in any given frame, apply the transformation by the value * degree of knob value
    // call draw frame number of times, once for each frame (the below thing is draw)

    for (int i = 0; i < frames; i++) {

    	// reset file stream
    	cnt = 0;

        // reset zbuffer, cstack
        cstack = std::stack<matrix>();
        matrix M(4,4);
        ident(M);
        cstack.push(M);
        S.clear();
        std::fill(zbuffer.begin(), zbuffer.end(), std::vector<double>(N, -std::numeric_limits<double>::infinity()));

	    while (cnt < num_cmd) {
            cmd = (cmdlist[cnt].index() == 1 ? std::get<std::string>(cmdlist[cnt]) : "");
	        if (cmd == "push") {
	            ++cnt;
	            matrix m = cstack.top();
				cstack.push (m);
	        } else if (cmd == "pop") {
	            ++cnt;
	            cstack.pop ();
	        } else if (cmd == "move") {
	            double x = std::get<double>(cmdlist[cnt + 1]), y = std::get<double>(cmdlist[cnt + 2]), z = std::get<double>(cmdlist[cnt + 3]);
                knob = std::get<std::string>(cmdlist[cnt + 4]);
	            cnt += 4;
	            if (knob != "None") {
	            	x *= frame_list[i][knob];
	            	y *= frame_list[i][knob];
	            	z *= frame_list[i][knob];
	            }
				cstack.top() *= move (x, y, z);
	        } else if (cmd == "rotate") {
	            std::string axis = std::get<std::string>(cmdlist[cnt + 1]);
	            double theta = std::get<double>(cmdlist[cnt + 2]);
                knob = std::get<std::string>(cmdlist[cnt + 3]);
	            cnt += 3;
	            if (knob != "None") {
	            	theta *= frame_list[i][knob];
	            }
	            if (axis == "x") {
	                cstack.top() *= rot_x (theta);
	            } else if (axis == "y") {
	                cstack.top() *= rot_y (theta);
	            } else if (axis == "z") {
	                cstack.top() *= rot_z (theta);
	            }
	        } else if (cmd == "scale") {
                double x = std::get<double>(cmdlist[cnt + 1]), y = std::get<double>(cmdlist[cnt + 2]), z = std::get<double>(cmdlist[cnt + 3]);
                knob = std::get<std::string>(cmdlist[cnt + 4]);
                cnt += 4;
	            if (knob != "None") {
	            	x *= frame_list[i][knob];
	            	y *= frame_list[i][knob];
	            	z *= frame_list[i][knob];
	            }
	            cstack.top() *= scale (x, y, z);
	        } else if (cmd == "box") {
	            std::string constants = std::get<std::string>(cmdlist[cnt + 1]);
	            ++cnt;
	            double x = std::get<double>(cmdlist[cnt + 2]),
                y = std::get<double>(cmdlist[cnt + 3]),
                z = std::get<double>(cmdlist[cnt + 4]),
                width = std::get<double>(cmdlist[cnt + 5]),
                height = std::get<double>(cmdlist[cnt + 6]),
                depth = std::get<double>(cmdlist[cnt + 7]);
				cnt += 6;
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
	            std::string constants = std::get<std::string>(cmdlist[cnt + 1]);
	            ++cnt;
	            double x = std::get<double>(cmdlist[cnt + 2]),
                y = std::get<double>(cmdlist[cnt + 3]),
                z = std::get<double>(cmdlist[cnt + 4]),
                r = std::get<double>(cmdlist[cnt + 5]);
                cnt += 4;
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
                std::string constants = std::get<std::string>(cmdlist[cnt + 1]);
	            ++cnt;
	            double x = std::get<double>(cmdlist[cnt + 2]),
                y = std::get<double>(cmdlist[cnt + 3]),
                z = std::get<double>(cmdlist[cnt + 4]),
                r1 = std::get<double>(cmdlist[cnt + 5]),
                r2 = std::get<double>(cmdlist[cnt + 6]);
                cnt += 5;
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

				draw_lines (T, S, WHITE, zbuffer, ambient, lights, ka_r, kd_r, ks_r, ka_g, kd_g, ks_g, ka_b, kd_b, ks_b);
				T.clear();
	        } else if (cmd == "line") {
	            ++cnt;
	            double x0 = std::get<double>(cmdlist[cnt + 1]),
                y0 = std::get<double>(cmdlist[cnt + 2]),
                z0 = std::get<double>(cmdlist[cnt + 3]),
                x1 = std::get<double>(cmdlist[cnt + 4]),
                y1 = std::get<double>(cmdlist[cnt + 5]),
                z1 = std::get<double>(cmdlist[cnt + 6]);
                cnt += 6;
				E.add_edge (x0, y0, z0, x1, y1, z1);
				E *= cstack.top();
				draw_lines (E, S, WHITE, zbuffer);
				E.clear();
	        }
            ++cnt;
	        /*
	        else if (cmd == "save") {
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
	        */
	        // std::cout << cmd << '\n';
	    }

        // save
        std::string num = std::to_string(i);
        for (int j = 0; j < 3 - std::to_string(i).size(); j++)
            num = "0" + num;
        std::string fname = "img/" + basename + num;
        S.to_ppm (fname);
        /*
        std::string fsave = "convert " + fname + ".ppm " + fname;
        std::system (fsave.c_str ());
        std::string fdel = "rm " + fname + ".ppm";
        std::system (fdel.c_str ());
        */

	    // update knob values in the symbol table
        for (auto& p : frame_list[i]) {
        	symtab[p.first].val = p.second;
        }

        std::printf("Created image %d of %d\n", i+1, frames);
	}


	std::string conv = "convert img/* -delay 1.7 img/" + basename + ".gif";
	std::system (conv.c_str());

    symin.close();
   	cmdin.close();

}

#pragma once

#include "matrix.hpp"
#include "screen.hpp"

enum illuminationmode {
    CEL, PHONG
};

enum shadingmode {
    FLAT_SHADING, GOURAUD_SHADING, PHONG_SHADING
};

void draw_line (int x0, int y0, double z0, int x1, int y1, double z1, picture& p, const color& c, std::vector<std::vector<double>>& zbuffer);
void phong_scanline (int x0, double z0, int x1, double z1, int y,
			picture& p, std::vector<std::vector<double>>& zbuffer,
			point left, point right,
			const color& ambient, std::vector<light>& lights,
			double ka_r, double kd_r, double ks_r,
			double ka_g, double kd_g, double ks_g,
			double ka_b, double kd_b, double ks_b);
void draw_lines (edgelist& points, picture& p, const color& c, std::vector<std::vector<double>>& zbuffer);

void draw_lines_flat (trianglelist& points, picture& p, illuminationmode shading_mode, std::vector<std::vector<double>>& zbuffer,
                 const color& ambient, std::vector<light>& lights, 

                 double ka_r, double kd_r, double ks_r,
                 double ka_g, double kd_g, double ks_g,
                 double ka_b, double kd_b, double ks_b);
void phong_draw_lines (trianglelist& points, picture& p, illuminationmode shading_mode, std::vector<std::vector<double>>& zbuffer,
				 const color& ambient, std::vector<light>& lights,
				 double ka_r, double kd_r, double ks_r,
				 double ka_g, double kd_g, double ks_g,
				 double ka_b, double kd_b, double ks_b);

color get_phong_color (double x1, double y1, double z1,
                 double x2, double y2, double z2,
                 double x3, double y3, double z3,
                 const color& ambient, std::vector<light>& lights,
                 double ka_r, double kd_r, double ks_r,
                 double ka_g, double kd_g, double ks_g,
                 double ka_b, double kd_b, double ks_b);


color get_cel_color (double x1, double y1, double z1,
                 double x2, double y2, double z2,
                 double x3, double y3, double z3,
                 const color& ambient, std::vector<light>& lights,
                 double ka_r, double kd_r, 
                 double ka_g, double kd_g, 
                 double ka_b, double kd_b);

void draw_lines_gouraud (trianglelist& points, picture& p, illuminationmode shading_mode, std::vector<std::vector<double>>& zbuffer,
                 const color& ambient, std::vector<light>& lights, 
                 double ka_r, double kd_r, double ks_r,
                 double ka_g, double kd_g, double ks_g,
                 double ka_b, double kd_b, double ks_b);

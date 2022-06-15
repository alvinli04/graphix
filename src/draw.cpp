#include <algorithm>
#include <iostream>
#include <cassert>

#include "matrix.hpp"
#include "screen.hpp"
#include "draw.hpp"
#include "color_constants.hpp"


// Draws a line from (x0, y0) to (x1, y1)
void draw_line (int x0, int y0, double z0, int x1, int y1, double z1, picture& p, const color& c, std::vector<std::vector<double>>& zbuffer) {
	// starting point is always left of the ending point
	if (x0 > x1) {
		std::swap (x0, x1);
		std::swap (y0, y1);
		std::swap (z0, z1);
	}

	int x = x0;
	int y = y0;
	double z = z0;
	int A = y1 - y0;
	int B = x0 - x1;

	if (y1 >= y0 && A <= B * -1) {
		// octant 1
		int D = 2 * A + B;
		double dz = B != 0 ? (z0 - z1) / (B + 1) : 0;

		while (x <= x1) {
			if (0 <= y && y < p.height && 0 <= x && x < p.width && (int)(z * 10) / 10.0 >= zbuffer[y][x]) {
				p[y][x].set(c);
				zbuffer[y][x] = (int)(z * 10) / 10.0;
			}
			if (D > 0) {
				D += 2 * B;
				++y;
			}

			++x;
			z += dz;

			D += 2 * A;
		}
	} else if (y1 >= y0) {
		// octant 2
		int D = A + 2 * B;
		double dz = A != 0 ? (z1 - z0) / (A + 1) : 0;

		while (y <= y1) {
			if (0 <= y && y < p.height && 0 <= x && x < p.width && (int)(z * 10) / 10.0 >= zbuffer[y][x]) {
				p[y][x].set(c);
				zbuffer[y][x] = (int)(z * 10) / 10.0;
			}
			if (D < 0) {
				D += 2 * A;
				++x;
			}

			++y;
			z += dz;

			D += 2 * B;
		}
	} else if (A >= B) {
		// octant 8
		int D = 2 * A - B;
		double dz = B != 0 ? (z0 - z1) / (B + 1) : 0;

		while (x <= x1) {
			if (0 <= y && y < p.height && 0 <= x && x < p.width && (int)(z * 10) / 10.0 >= zbuffer[y][x]) {
				p[y][x].set(c);
				zbuffer[y][x] = (int)(z * 10) / 10.0;
			}
			if (D < 0) {
				D -= 2 * B;
				--y;
			}

			++x;
			z += dz;

			D += 2 * A;
		}
	} else {
		// octant 7
		int D = A - 2 * B;
		double dz = A != 0 ? (z1 - z0) / (A + 1) : 0;

		while (y >= y1) {
			if (0 <= y && y < p.height && 0 <= x && x < p.width && (int)(z * 10) / 10.0 >= zbuffer[y][x]) {
				p[y][x].set(c);
				zbuffer[y][x] = (int)(z * 10) / 10.0;
			}
			if (D > 0) {
				D += 2 * A;
				++x;
			}

			--y;
			z += dz;

			D -= 2 * B;
		}
	}
}


// Draws lines from an edge list
void draw_lines (edgelist& points, picture& p, const color& c, std::vector<std::vector<double>>& zbuffer) {
	for (int i = 0; i < points.cols; i += 2) {
		draw_line (points[0][i], points[1][i], points[2][i], points[0][i+1], points[1][i+1], points[2][i+1], p, c, zbuffer);
	}
}


// Get color based on the phong reflection model
color get_phong_color (double x1, double y1, double z1,
				 double x2, double y2, double z2,
				 double x3, double y3, double z3,
				 const color& ambient, std::vector<light>& lights,
				 double ka_r, double kd_r, double ks_r,
				 double ka_g, double kd_g, double ks_g,
				 double ka_b, double kd_b, double ks_b) {
	// Get the normal
	double ax = x2 - x1, ay = y2 - y1, az = z2 - z1;
	double bx = x3 - x1, by = y3 - y1, bz = z3 - z1;
	double Nx = ay * bz - az * by;
	double Ny = az * bx - ax * bz;
	double Nz = ax * by - ay * bx;
	double Nnorm = std::sqrt (Nx * Nx + Ny * Ny + Nz * Nz);
	// normalize
	Nx /= Nnorm, Ny /= Nnorm, Nz /= Nnorm;

	// ambient light
	color c_f (ka_r * ambient.red, ka_g * ambient.green, ka_b * ambient.blue);

	for (light& l : lights) {
		// get L vector
		double lx = l.x;
		double ly = l.y;
		double lz = l.z;
		double lnorm = std::sqrt (l.x * l.x + l.y * l.y + l.z * l.z);
		lx /= lnorm, ly /= lnorm, lz /= lnorm;

		// diffuse
		double cct = lx * Nx + ly * Ny + lz * Nz;
		double ct = std::max (0.0, cct);

		double cdr = kd_r * ct * l.c.red;
		double cdg = kd_g * ct * l.c.green;
		double cdb = kd_b * ct * l.c.blue;

		c_f.red   = (cdr < 255 - c_f.red   ? c_f.red   + cdr : 255);
		c_f.green = (cdg < 255 - c_f.green ? c_f.green + cdg : 255);
		c_f.blue  = (cdb < 255 - c_f.blue  ? c_f.blue  + cdb : 255);

		// specular
		double rfz = std::max (0.0, 2 * ct * Nz - lz);
		rfz = rfz * rfz * rfz;

		cdr = ks_r * rfz * l.c.red;
		cdg = ks_g * rfz * l.c.green;
		cdb = ks_b * rfz * l.c.blue;

		c_f.red   = (cdr < 255 - c_f.red   ? c_f.red   + cdr : 255);
		c_f.green = (cdg < 255 - c_f.green ? c_f.green + cdg : 255);
		c_f.blue  = (cdb < 255 - c_f.blue  ? c_f.blue  + cdb : 255);
	}

	return c_f;
}

// Get color based on the phong reflection model
color get_phong_color_from_norm (double Nx, double Ny, double Nz,
				 const color& ambient, std::vector<light>& lights,
				 double ka_r, double kd_r, double ks_r,
				 double ka_g, double kd_g, double ks_g,
				 double ka_b, double kd_b, double ks_b) {
	// Get the normal
	double Nnorm = std::sqrt (Nx * Nx + Ny * Ny + Nz * Nz);
	// normalize
	Nx /= Nnorm, Ny /= Nnorm, Nz /= Nnorm;

	// ambient light
	color c_f (ka_r * ambient.red, ka_g * ambient.green, ka_b * ambient.blue);

	for (light& l : lights) {
		// get L vector
		double lx = l.x;
		double ly = l.y;
		double lz = l.z;
		double lnorm = std::sqrt (l.x * l.x + l.y * l.y + l.z * l.z);
		lx /= lnorm, ly /= lnorm, lz /= lnorm;

		// diffuse
		double cct = lx * Nx + ly * Ny + lz * Nz;
		double ct = std::max (0.0, cct);

		double cdr = kd_r * ct * l.c.red;
		double cdg = kd_g * ct * l.c.green;
		double cdb = kd_b * ct * l.c.blue;

		c_f.red   = (cdr < 255 - c_f.red   ? c_f.red   + cdr : 255);
		c_f.green = (cdg < 255 - c_f.green ? c_f.green + cdg : 255);
		c_f.blue  = (cdb < 255 - c_f.blue  ? c_f.blue  + cdb : 255);

		// specular
		double rfz = std::max (0.0, 2 * ct * Nz - lz);
		rfz = rfz * rfz * rfz;

		cdr = ks_r * rfz * l.c.red;
		cdg = ks_g * rfz * l.c.green;
		cdb = ks_b * rfz * l.c.blue;

		c_f.red   = (cdr < 255 - c_f.red   ? c_f.red   + cdr : 255);
		c_f.green = (cdg < 255 - c_f.green ? c_f.green + cdg : 255);
		c_f.blue  = (cdb < 255 - c_f.blue  ? c_f.blue  + cdb : 255);
	}

	return c_f;
}

// Get color based a cel shading model
// FIXME: Figue out why this is so dim
color get_cel_color (double x1, double y1, double z1,
				 double x2, double y2, double z2,
				 double x3, double y3, double z3,
				 const color& ambient, std::vector<light>& lights,
				 double ka_r, double kd_r,
				 double ka_g, double kd_g,
				 double ka_b, double kd_b) {
	// Get the normal
	double ax = x2 - x1, ay = y2 - y1, az = z2 - z1;
	double bx = x3 - x1, by = y3 - y1, bz = z3 - z1;
	double Nx = ay * bz - az * by;
	double Ny = az * bx - ax * bz;
	double Nz = ax * by - ay * bx;
	double Nnorm = std::sqrt (Nx * Nx + Ny * Ny + Nz * Nz);
	// normalize
	Nx /= Nnorm, Ny /= Nnorm, Nz /= Nnorm;

	// ambient light
	color c_f (ka_r * ambient.red, ka_g * ambient.green, ka_b * ambient.blue);

	for (light& l : lights) {
		// get L vector
		double lx = l.x;
		double ly = l.y;
		double lz = l.z;
		double lnorm = std::sqrt (l.x * l.x + l.y * l.y + l.z * l.z);
		lx /= lnorm, ly /= lnorm, lz /= lnorm;

		// diffuse
		double cct = lx * Nx + ly * Ny + lz * Nz;
		double ct = std::max (0.0, cct);

        // cases for the different possible threshholds
        // please adjust the constants as desired
        double diffuse_mult;
        if (ct > 0.95) {
            diffuse_mult = 1.0;
        } else if (ct > 0.5) {
            diffuse_mult = 0.95;
        } else if (ct > 0.25) {
            diffuse_mult = 0.5;
        } else {
            diffuse_mult = 0.0;
        }

		double cdr = kd_r * diffuse_mult * l.c.red;
		double cdg = kd_g * diffuse_mult * l.c.green;
		double cdb = kd_b * diffuse_mult * l.c.blue;

		c_f.red   = (cdr < 255 - c_f.red   ? c_f.red   + cdr : 255);
		c_f.green = (cdg < 255 - c_f.green ? c_f.green + cdg : 255);
		c_f.blue  = (cdb < 255 - c_f.blue  ? c_f.blue  + cdb : 255);

		// specular
		double rfz = std::max (0.0, 2 * ct * Nz - lz);
		rfz = rfz * rfz * rfz;
        if (rfz >= 0.8) {
            c_f.red = 255;
            c_f.green = 255;
            c_f.blue = 255;
        }
	}

	return c_f;
}
// Get color based a cel shading model
// FIXME: Figue out why this is so dim
color get_cel_color_from_norm (double Nx, double Ny, double Nz,
				 const color& ambient, std::vector<light>& lights,
				 double ka_r, double kd_r,
				 double ka_g, double kd_g,
				 double ka_b, double kd_b) {
	// Get the normal
	double Nnorm = std::sqrt (Nx * Nx + Ny * Ny + Nz * Nz);
	// normalize
	Nx /= Nnorm, Ny /= Nnorm, Nz /= Nnorm;

	// ambient light
	color c_f (ka_r * ambient.red, ka_g * ambient.green, ka_b * ambient.blue);

	for (light& l : lights) {
		// get L vector
		double lx = l.x;
		double ly = l.y;
		double lz = l.z;
		double lnorm = std::sqrt (l.x * l.x + l.y * l.y + l.z * l.z);
		lx /= lnorm, ly /= lnorm, lz /= lnorm;

		// diffuse
		double cct = lx * Nx + ly * Ny + lz * Nz;
		double ct = std::max (0.0, cct);

        // cases for the different possible threshholds
        // please adjust the constants as desired
        double diffuse_mult;
        if (ct > 0.95) {
            diffuse_mult = 1.0;
        } else if (ct > 0.5) {
            diffuse_mult = 0.95;
        } else if (ct > 0.25) {
            diffuse_mult = 0.5;
        } else {
            diffuse_mult = 0.0;
        }

		double cdr = kd_r * diffuse_mult * l.c.red;
		double cdg = kd_g * diffuse_mult * l.c.green;
		double cdb = kd_b * diffuse_mult * l.c.blue;

		c_f.red   = (cdr < 255 - c_f.red   ? c_f.red   + cdr : 255);
		c_f.green = (cdg < 255 - c_f.green ? c_f.green + cdg : 255);
		c_f.blue  = (cdb < 255 - c_f.blue  ? c_f.blue  + cdb : 255);

		// specular
		double rfz = std::max (0.0, 2 * ct * Nz - lz);
		rfz = rfz * rfz * rfz;
        if (rfz >= 0.8) {
            c_f.red = 255;
            c_f.green = 255;
            c_f.blue = 255;
        }
	}

	return c_f;
}


// Draws lines from a triangle list
void draw_lines_flat (trianglelist& points, picture& p, illuminationmode shading_mode, std::vector<std::vector<double>>& zbuffer,
				 const color& ambient, std::vector<light>& lights,
				 double ka_r, double kd_r, double ks_r,
				 double ka_g, double kd_g, double ks_g,
				 double ka_b, double kd_b, double ks_b) {
	for (int i = 0; i < points.cols; i += 3) {
		double x1 = points[0][i]  , y1 = points[1][i]  , z1 = points[2][i]  ,
			   x2 = points[0][i+1], y2 = points[1][i+1], z2 = points[2][i+1],
			   x3 = points[0][i+2], y3 = points[1][i+2], z3 = points[2][i+2];

		double dx1 = x2 - x1, dx2 = x3 - x1,
			   dy1 = y2 - y1, dy2 = y3 - y1;

		if (dx1 * dy2 > dx2 * dy1) {

			// color calculation
            color c_f;
            switch (shading_mode) {
                case CEL:
                    c_f = get_cel_color (x1, y1, z1, x2, y2, z2, x3, y3, z3, ambient, lights, ka_r, kd_r, ka_g, kd_g, ka_b, kd_b);
                    break;
                case PHONG:
                    c_f = get_phong_color (x1, y1, z1, x2, y2, z2, x3, y3, z3, ambient, lights, ka_r, kd_r, ks_r, ka_g, kd_g, ks_g,ka_b, kd_b, ks_b);
                    break;
                default:
                    std::cerr << "unrecognized shading enum, this is a problem in draw.cpp or main.cpp and is a bad thing, please fix me" << std::endl;
                    c_f = get_phong_color (x1, y1, z1, x2, y2, z2, x3, y3, z3, ambient, lights, ka_r, kd_r, ks_r, ka_g, kd_g, ks_g,ka_b, kd_b, ks_b);
                    break;
            }

			// scanline conversion
			if (y1 < y2) {
				std::swap (y1, y2);
				std::swap (x1, x2);
				std::swap (z1, z2);
			}
			if (y1 < y3) {
				std::swap (y1, y3);
				std::swap (x1, x3);
				std::swap (z1, z3);
			}
			if (y2 < y3) {
				std::swap (y2, y3);
				std::swap (x2, x3);
				std::swap (z2, z3);
			} // values are sorted in order y1, y2, y3

			double bx0 = x3; // goes along BT
			double bx1 = x3; // bx1 goes along the other 2 edges
			double bz0 = z3;
			double bz1 = z3;
			int y = y3;

			double slopeBT = y1 - y3 + 1 > 0 ? (x1 - x3) / (y1 - y3 + 1) : 0;
			double slopeBM = y2 - y3 + 1 > 0 ? (x2 - x3) / (y2 - y3 + 1) : 0;
			double slopeMT = y1 - y2 + 1 > 0 ? (x1 - x2) / (y1 - y2 + 1) : 0;

			double slopeBTz = y1 - y3 + 1 > 0 ? (z1 - z3) / (y1 - y3 + 1) : 0;
			double slopeBMz = y2 - y3 + 1 > 0 ? (z2 - z3) / (y2 - y3 + 1) : 0;
			double slopeMTz = y1 - y2 + 1 > 0 ? (z1 - z2) / (y1 - y2 + 1) : 0;

			bool flip = 0;
			double dx0 = slopeBT;
			double dx1 = slopeBM;
			double dz0 = slopeBTz;
			double dz1 = slopeBMz;


			while (y <= (int)y1) {
				if (!flip && y >= (int)y2) {
					flip = 1;
					dx1 = slopeMT;
					dz1 = slopeMTz;
					bx1 = x2;
					bz1 = z2;
				}

				draw_line (bx0, y, bz0, bx1, y, bz1, p, c_f, zbuffer);
				bx0 += dx0;
				bx1 += dx1;
				bz0 += dz0;
				bz1 += dz1;
				++y;
			}

			// draw triangle boundaries
			// draw_line (x1, y1, z1, x2, y2, z2, p, CYAN, zbuffer);
			// draw_line (x2, y2, z2, x3, y3, z3, p, CYAN, zbuffer);
			// draw_line (x3, y3, z3, x1, y1, z1, p, CYAN, zbuffer);
		}
	}
}

// scanline for phong shading model
// left and right are edge norms along the lines
void phong_scanline (int x0, double z0, int x1, double z1, int y,
			picture& p, std::vector<std::vector<double>>& zbuffer,
			point left, point right,
			const color& ambient, std::vector<light>& lights,
			double ka_r, double kd_r, double ks_r,
			double ka_g, double kd_g, double ks_g,
			double ka_b, double kd_b, double ks_b,
			illuminationmode shading_mode) {

	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(z0, z1);
		std::swap(left, right);
	}
	double dz = x1 != x0 ? (z1 - z0) / (x1 - x0 + 1) : 0;
	double z = z0;
	point N = left; // the interpolated norm
	point dN = x1 != x0 ? (right - left) / (x1 - x0 + 1) : point{0, 0, 0};
	//std::printf("left: %f %f %f\n", left.x, left.y, left.z);
	//std::printf("right: %f %f %f\n", right.x, right.y, right.z);
	for (int i = x0; i <= x1; ++i) {
		if (0 <= y && y < p.height && 0 <= i && i < p.width && (int)(z * 10) / 10.0 >= zbuffer[y][i]) {
			// color calculation
            color c_f;
            switch (shading_mode) {
                case CEL:
                    c_f = get_cel_color_from_norm (N.x, N.y, N.z, ambient, lights, ka_r, kd_r, ka_g, kd_g, ka_b, kd_b);
                    break;
                case PHONG:
                    c_f = get_phong_color_from_norm (N.x, N.y, N.z, ambient, lights, ka_r, kd_r, ks_r, ka_g, kd_g, ks_g, ka_b, kd_b, ks_b);
                    break;
                default:
                    std::cerr << "unrecognized shading enum, this is a problem in draw.cpp or main.cpp and is a bad thing, please fix me" << std::endl;
					c_f = get_phong_color_from_norm (N.x, N.y, N.z, ambient, lights, ka_r, kd_r, ks_r, ka_g, kd_g, ks_g, ka_b, kd_b, ks_b);
                    break;
            }

			p[y][i].set(c_f);
			// std::printf("%f %f %f\n", left.x, left.y, left.z);
			zbuffer[y][i] = (int)(z * 10) / 10.0;

		}
		z += dz;
		N += dN;
	}
}

// Draws lines from a triangle list
void phong_draw_lines (trianglelist& points, picture& p, illuminationmode shading_mode, std::vector<std::vector<double>>& zbuffer,
				 const color& ambient, std::vector<light>& lights,
				 double ka_r, double kd_r, double ks_r,
				 double ka_g, double kd_g, double ks_g,
				 double ka_b, double kd_b, double ks_b) {
	for (int i = 0; i < points.cols; i += 3) {
		double x1 = points[0][i]  , y1 = points[1][i]  , z1 = points[2][i]  ,
			   x2 = points[0][i+1], y2 = points[1][i+1], z2 = points[2][i+1],
			   x3 = points[0][i+2], y3 = points[1][i+2], z3 = points[2][i+2];

		double dx1 = x2 - x1, dx2 = x3 - x1,
			   dy1 = y2 - y1, dy2 = y3 - y1;

		if (dx1 * dy2 > dx2 * dy1) {

			// scanline conversion
			if (y1 < y2) {
				std::swap (y1, y2);
				std::swap (x1, x2);
				std::swap (z1, z2);
			}
			if (y1 < y3) {
				std::swap (y1, y3);
				std::swap (x1, x3);
				std::swap (z1, z3);
			}
			if (y2 < y3) {
				std::swap (y2, y3);
				std::swap (x2, x3);
				std::swap (z2, z3);
			} // values are sorted in order y1, y2, y3

			double bx0 = x3; // goes along BT
			double bx1 = x3; // bx1 goes along the other 2 edges
			double bz0 = z3;
			double bz1 = z3;
			int y = y3;

			point N1 = points.get_vertex_normal(x1, y1, z1);
			point N2 = points.get_vertex_normal(x2, y2, z2);
			point N3 = points.get_vertex_normal(x3, y3, z3);
			point bp0 = N3;
			point bp1 = N3;
			// std::printf("N1: %f %f %f\n", N1.x, N1.y, N1.z);
			// std::printf("N2: %f %f %f\n", N2.x, N2.y, N2.z);
			// std::printf("N3: %f %f %f\n", N3.x, N3.y, N3.z);
			// std::printf("N1 - N3: %f %f %f %d\n", (N1-N3).x, (N1-N3).y, (N1-N3).z, y1 - y3 + 1);

			point slopeBTp = y1 - y3 + 1 > 0 ? (N1 - N3) / (y1 - y3 + 1) : point{0,0,0};
			point slopeBMp = y2 - y3 + 1 > 0 ? (N2 - N3) / (y2 - y3 + 1) : point{0,0,0};
			point slopeMTp = y1 - y2 + 1 > 0 ? (N1 - N2) / (y1 - y2 + 1) : point{0,0,0};

			double slopeBT = y1 - y3 + 1 > 0 ? (x1 - x3) / (y1 - y3 + 1) : 0;
			double slopeBM = y2 - y3 + 1 > 0 ? (x2 - x3) / (y2 - y3 + 1) : 0;
			double slopeMT = y1 - y2 + 1 > 0 ? (x1 - x2) / (y1 - y2 + 1) : 0;

			double slopeBTz = y1 - y3 + 1 > 0 ? (z1 - z3) / (y1 - y3 + 1) : 0;
			double slopeBMz = y2 - y3 + 1 > 0 ? (z2 - z3) / (y2 - y3 + 1) : 0;
			double slopeMTz = y1 - y2 + 1 > 0 ? (z1 - z2) / (y1 - y2 + 1) : 0;

			bool flip = 0;
			double dx0 = slopeBT;
			double dx1 = slopeBM;
			double dz0 = slopeBTz;
			double dz1 = slopeBMz;
			point dp0 = slopeBTp;
			point dp1 = slopeBMp;
			// std::printf("slope: %f %f %f\n", dp0.x, dp0.y, dp0.z);
			while (y <= (int)y1) {
				if (!flip && y >= (int)y2) {
					flip = 1;
					dx1 = slopeMT;
					dz1 = slopeMTz;
					dp1 = slopeMTp;
					bx1 = x2;
					bz1 = z2;
					bp1 = N2;
				}

				phong_scanline (bx0, bz0, bx1, bz1, y,
					p, zbuffer,
					bp0, bp1,
					ambient, lights,
					ka_r, kd_r, ks_r,
					ka_g, kd_g, ks_g,
					ka_b, kd_b, ks_b,
					shading_mode);
				bx0 += dx0;
				bx1 += dx1;
				bz0 += dz0;
				bz1 += dz1;
				bp0 += dp0;
				bp1 += dp1;
				++y;
			}

			// draw triangle boundaries
			// draw_line (x1, y1, z1, x2, y2, z2, p, CYAN, zbuffer);
			// draw_line (x2, y2, z2, x3, y3, z3, p, CYAN, zbuffer);
			// draw_line (x3, y3, z3, x1, y1, z1, p, CYAN, zbuffer);
		}
	}
}

void draw_lines_gouraud (trianglelist& points, picture& p, illuminationmode shading_mode, std::vector<std::vector<double>>& zbuffer,
                 const color& ambient, std::vector<light>& lights, 
                 double ka_r, double kd_r, double ks_r,
                 double ka_g, double kd_g, double ks_g,
                 double ka_b, double kd_b, double ks_b) {
    // iterate over triangles
	for (int i = 0; i < points.cols; i += 3) {
        double 
            x1 = points[0][i]  , y1 = points[1][i]  , z1 = points[2][i]  ,
            x2 = points[0][i+1], y2 = points[1][i+1], z2 = points[2][i+1],
            x3 = points[0][i+2], y3 = points[1][i+2], z3 = points[2][i+2];
        // the three steps of an insertion sort to sort
        if (y2 < y1) {
            std::swap(x2, x1);
            std::swap(y2, y1);
            std::swap(z2, z1);
        }
        if (y3 < y2) {
            std::swap(x2, x3);
            std::swap(y2, y3);
            std::swap(z2, z3);
        }
        if (y2 < y1) {
            std::swap(x2, x1);
            std::swap(y2, y1);
            std::swap(z2, z1);
        }
        const double EPSILON = 0.0001;
        if (abs(y1-y2) < EPSILON && abs(y2-y3) < EPSILON) {
            return;
        }

        color c1, c2, c3;
        point 
            nbot = points.get_vertex_normal(x1, y1, z1), 
            nmid = points.get_vertex_normal(x2, y2, z2),
            ntop = points.get_vertex_normal(x3, y3, z3);
        switch (shading_mode) {
            case CEL:
                c1 = get_cel_color_from_norm(nbot.x, nbot.y, nbot.z, ambient, lights, ka_r, kd_r, ka_g, kd_g, ka_b, kd_b);
                c2 = get_cel_color_from_norm(nmid.x, nmid.y, nmid.z, ambient, lights, ka_r, kd_r, ka_g, kd_g, ka_b, kd_b);
                c3 = get_cel_color_from_norm(ntop.x, ntop.y, ntop.z, ambient, lights, ka_r, kd_r, ka_g, kd_g, ka_b, kd_b);
                break;
            case PHONG:
                c1 = get_phong_color_from_norm(nbot.x, nbot.y, nbot.z, ambient, lights, ka_r, kd_r, ks_r, ka_g, kd_g, ks_g, ka_b, kd_b, ks_b);
                c2 = get_phong_color_from_norm(nmid.x, nmid.y, nmid.z, ambient, lights, ka_r, kd_r, ks_r, ka_g, kd_g, ks_g, ka_b, kd_b, ks_b);
                c3 = get_phong_color_from_norm(ntop.x, ntop.y, ntop.z, ambient, lights, ka_r, kd_r, ks_r, ka_g, kd_g, ks_g, ka_b, kd_b, ks_b);
                break;
        }

        double
            dx12 = (x2 - x1) / (y2 - y1),
            dz12 = (z2 - z1) / (y2 - y1),
            dc12red = (c2.red - c1.red) / (y2 - y1),
            dc12green = (c2.green - c1.green) / (y2 - y1),
            dc12blue = (c2.blue - c1.blue) / (y2 - y1),
            dx23 = (x3 - x2) / (y3 - y2),
            dz23 = (z3 - z2) / (y3 - y2),
            dc23red = (c3.red - c2.red) / (y3 - y2),
            dc23green = (c3.green - c2.green) / (y3 - y2),
            dc23blue = (c3.blue - c2.blue) / (y3 - y2),
            dx13 = (x3 - x1) / (y3 - y1),
            dz13 = (z3 - z1) / (y3 - y1),
            dc13red = (c3.red - c1.red) / (y3 - y1),
            dc13green = (c3.green - c1.green) / (y3 - y1),
            dc13blue = (c3.blue - c1.blue) / (y3 - y1);
        
        bool right_scanline_direction = dx12 < dx13;

        double
            xa = x1 + dx12 * (floor(y1) + 1.0 - y1),
            za = z1 + dz12 * (floor(y1) + 1.0 - y1),
            cared = c1.red + dc12red * (floor(y1) + 1.0 - y1),
            cagreen = c1.green + dc12green * (floor(y1) + 1.0 - y1),
            cablue = c1.blue + dc12blue * (floor(y1) + 1.0 - y1),
            xb = x1 + dx13 * (floor(y1) + 1.0 - y1),
            zb = x1 + dz13 * (floor(y1) + 1.0 - y1),
            cbred = c1.red + dc13red * (floor(y1) + 1.0 - y1),
            cbgreen = c1.green + dc13green * (floor(y1) + 1.0 - y1),
            cbblue = c1.blue + dc13blue * (floor(y1) + 1.0 - y1);


        auto scanline = [&p, &zbuffer](double c1red, double c1green, double c1blue, double c2red, double c2green, double c2blue, int y, double x0, double x1, double z0, double z1) -> void {
            for (int x = floor(x0) + 1; x <= floor(x1); x++) {
                double z = z0 + (z1 - z0) * (x - x0) / (x1 - x0);
                if (0 <= y && y < p.height && 0 <= x && x < p.width && (int) (z * 10) / 10.0 >= zbuffer[y][x]) {
                    p[y][x].set(color { 
                            (uint8_t) (c1red + (c2red - c1red) * (x - x0) / (x1 - x0)),
                            (uint8_t) (c1green + (c2green - c1green) * (x - x0) / (x1 - x0)),
                            (uint8_t) (c1blue + (c2blue - c1blue) * (x - x0) / (x1 - x0)),
                    });
                    zbuffer[y][x] = (int) (z * 10) / 10.0;
                }
            }
        };

        for (int y = floor(y1) + 1; y <= floor(y2); y++) {
            if (right_scanline_direction) {
                scanline(cared, cagreen, cablue, cbred, cbgreen, cbblue, y, xa, xb, za, zb);
            } else {
                scanline(cbred, cbgreen, cbblue, cared, cagreen, cablue, y, xb, xa, zb, za);
            }
            xa += dx12;
            za += dz12;
            cared += dc12red;
            cagreen += dc12green;
            cablue += dc12blue;
            xb += dx13;
            zb += dz13;
            cbred += dc13red;
            cbgreen += dc13green;
            cbblue += dc13blue;
        }

        xa = x2 + dx23 * (floor(y2) + 1.0 - y2);
        za = z2 + dz23 * (floor(y2) + 1.0 - y2);
        cared = c2.red + dc23red * (floor(y2) + 1.0 - y2);
        cagreen = c2.green + dc23green * (floor(y2) + 1.0 - y2);
        cablue = c2.blue + dc23blue * (floor(y2) + 1.0 - y2);

        for (int y = floor(y2) + 1.0; y <= (floor(y3) + 1.0); y++) {
            if (right_scanline_direction) {
                scanline(cared, cagreen, cablue, cbred, cbgreen, cbblue, y, xa, xb, za, zb);
            } else {
                scanline(cbred, cbgreen, cbblue, cared, cagreen, cablue, y, xb, xa, zb, za);
            }
            xa += dx23;
            za += dz23;
            cared += dc23red;
            cagreen += dc23green;
            cablue += dc23blue;
            xb += dx13;
            zb += dz13;
            cbred += dc13red;
            cbgreen += dc13green;
            cbblue += dc13blue;
        }
    }
}

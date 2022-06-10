#include <vector>
#include <fstream>
#include <ostream>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <string.h>
#include <cstdint>

#include "screen.hpp"
#include "color_constants.hpp"

// Color functions
// -----------------------------

// Constructors
color::color () {
	red = 0;
	green = 0;
	blue = 0;
}

color::color (int r, int g, int b) {
	red = r;
	green = g;
	blue = b;
}

// Sets rgb values for a color
void color::set (int r, int g, int b) {
	red = r;
	green = g;
	blue = b;
}

void color::set (const color& c) {
	red = c.red;
	green = c.green;
	blue = c.blue;
}

// prints the color in "red green blue" format for ppms
std::ostream& operator<<(std::ostream& os, color const& c){
	return (os << +c.red << " " << +c.green << " " << +c.blue);
}

//Light functions
// ------------------------------

//Constructor
light::light (double x, double y, double z, const color& c) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->c = c;
}

// prints a light
std::ostream& operator<<(std::ostream& os, light const& l){
	return (os << +l.x << " " << +l.y << " " << +l.z << " " << l.c);
}


//Picture functions
// ------------------------------

// Constructors
picture::picture () {
	width = 500;
	height = 500;
	grid.resize (height, std::vector<color> (width));
}

picture::picture (int n_width, int n_height) {
	width = n_width;
	height = n_height;
	grid.resize (height, std::vector<color> (width));
}

// Clears screen
void picture::clear () {
	for (auto& row : grid) {
		for (auto& c : row) {
			c.set(BLACK);
		}
	}
}

// Getter method
std::vector<color>& picture::operator[](int index) {
	assert (index < grid.size());
	return grid[index];
}

// Prints the entire picture in ppm format
std::ostream& operator<<(std::ostream& os, picture& p) {
	os << "P3\n" << p.height << " " << p.width << "\n255\n";

	for (int i = p.height - 1; i >= 0; --i) {
		for (int j = 0; j < p.width; ++j) {
			os << p[i][j] << " ";
		}
		if (i > 0) os << "\n";
	}
	return os;
}

// Converts the picture into a ppm
void picture::to_ppm (std::string s) {
	std::ofstream os;
	os.open(s + ".ppm");
	os << *this;
	os.close();
}

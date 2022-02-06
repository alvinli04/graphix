#include <vector>
#include <fstream>
#include <ostream>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <string.h>
#include "utils.h"

// Color functions

std::vector<int> color::to_vector() const {
	return {red, green, blue};
}

void color::set (int r, int g, int b) {
	red = r % 256;
	green = g % 256;
	blue = b % 256;
}

std::ostream& operator<<(std::ostream& os, color const& c){
	return (os << c.red << " " << c.green << " " << c.blue);
}


//Picture functions

picture::picture (int n_width, int n_height) {
	width = n_width;
	height = n_height;
	grid.resize(height, std::vector<color> (width));
}

std::vector<color>& picture::operator[](int index) {
	assert (index < grid.size());
	return grid[index];
}

std::ostream& operator<<(std::ostream& os, picture& p) {
	os << "P3\n" << p.height << " " << p.width << "\n255\n";

	for (int i = 0; i < p.height; ++i) {
		for (int j = 0; j < p.width; ++j) {
			os << p[i][j] << " ";
		}
		if (i < p.height - 1) os << "\n";
	}
	return os;
}



//Random util
void print (picture& p, std::string s) {
	std::ofstream os;
	os.open(s + ".ppm");
	os << p;
	os.close();
}
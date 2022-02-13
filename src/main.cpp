#include <cmath>
#include <iostream>
#include "screen.hpp"
#include "draw.hpp"
#include "color_constants.hpp"

#define N 1000
#define PI 3.1415926535

const color color_list[8] = {RED, LIME, BLUE, YELLOW, CYAN, MAGENTA, GREEN, TEAL};

void tree (int itr, int x, int y, double dir, int len, picture& p) {
	if (!itr) return;
	int newx = x + len * std::cos(dir), newy = y + len * std::sin(dir);
	color c;
	if (itr > 9) c = WHITE;
	else c = color_list[itr % 8];

	draw_line (x, y, newx, newy, p, c);
	
	tree (itr - 1, newx, newy, dir + PI / 6, len * 3 / 4, p);
	tree (itr - 1, newx, newy, dir - PI / 6, len * 3 / 4, p);
}

int main () {
	
    picture s(N, N);

	tree (15, 500, 150, PI/2, 200, s);

    s.to_ppm("test"); 
	
}

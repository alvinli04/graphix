#include <cmath>
#include <iostream>

#include "screen.hpp"
#include "draw.hpp"
#include "color_constants.hpp"
#include "matrix.hpp"

#define N 500
#define PI 3.1415926535



int main () {

    picture s (N, N);

	matrix T (4,4);
	ident (T);

    s.to_ppm("test");

}

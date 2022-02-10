#include <cmath>
#include <iostream>
#include "utils.hpp"
#include "color_constants.hpp"

int main () {
	/*
    picture p(1000, 1000);
	
    for (int i = 0; i < p.height; ++i) {
        for (int j = 0; j < p.width; ++j) {
            p[i][j].set(255*i*i/1000000, 255*j*j/1000000, 255*(i+j)/2000);
			
			if ((i - 250) * (i - 250) + (j - 250) * (j - 250) <= 120 * 120) {
				p[i][j].blue += (255 - p[i][j].blue) / 20;
			}
			if ((i - 800) * (i - 800) + (j - 200) * (j - 200) <= 60 * 60) {
				p[i][j].blue += (255 - p[i][j].blue) / 10;
			}
			if ((i - 650) * (i - 650) + (j - 600) * (j - 600) <= 90 * 90) {
				p[i][j].blue += (255 - p[i][j].blue) / 10;
			}
			if ((i - 600) * (i - 600) + (j - 400) * (j - 400) <= 150 * 150) {
				p[i][j].blue += (255 - p[i][j].blue) / 10;
			}

			if ((i - 100) * (i - 100) + (j - 900) * (j - 900) <= 300 * 300) {
				p[i][j].blue += (255 - p[i][j].blue) / 10;
			}

        }
    }
    
    p.to_ppm("test"); 
	*/

	picture p (500, 500);

	for (int i = 0; i < p.height; ++i)
		for (int j = 0; j < p.width; ++j)
			p[i][j].set(color(255, 59, 0));

	p.clear();
	
	p.to_ppm("test");

}

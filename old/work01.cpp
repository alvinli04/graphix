#include <cmath>
#include <iostream>
#include "screen.hpp"
#include "draw.hpp"
#include "color_constants.hpp"

#define XRES 500
#define YRES 500

int main () {
	
    picture s;
	color c;

	c.green = 255;
	
	//octants 1 and 5
  	draw_line(0, 0, XRES-1, YRES-1, s, c);
  	draw_line(0, 0, XRES-1, YRES / 2, s, c);
  	draw_line(XRES-1, YRES-1, 0, YRES / 2, s, c);	

	//octants 8 and 4
  	c.blue = 255;
  	draw_line(0, YRES-1, XRES-1, 0, s, c);
  	draw_line(0, YRES-1, XRES-1, YRES/2, s, c);
  	draw_line(XRES-1, 0, 0, YRES/2, s, c);
	
	//octants 2 and 6
	c.set(255, 0, 0);
  	draw_line(0, 0, XRES/2, YRES-1, s, c);
  	draw_line(XRES-1, YRES-1, XRES/2, 0, s, c);	
    
	//octants 7 and 3
  	c.blue = 255;
  	draw_line(0, YRES-1, XRES/2, 0, s, c);
  	draw_line(XRES-1, 0, XRES/2, YRES-1, s, c);

	//horizontal and vertical
  	c.blue = 0;
  	c.green = 255;
  	draw_line(0, YRES/2, XRES-1, YRES/2, s, c);
  	draw_line(XRES/2, 0, XRES/2, YRES-1, s, c);

    s.to_ppm("test"); 
	
}

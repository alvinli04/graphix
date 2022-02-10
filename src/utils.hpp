#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

class color {

public:
    int red;
    int green;
    int blue;
    
    color ();
	color (int r, int g, int b);
    
    void set (int r, int g, int b);
	void set (const color& c);
    
    friend std::ostream& operator<<(std::ostream& os, color const& c);

};


class picture {

public:
    int width;
    int height;
	std::vector<std::vector<color>> grid;

    picture (int width, int height);

    void clear ();
    void line (int x0, int y0, int x1, int y1, color c);

	std::vector<color>& operator[](int index);
    friend std::ostream& operator<<(std::ostream& os, picture const& p);
	void to_ppm (std::string s);

};  


#endif

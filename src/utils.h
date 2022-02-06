#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

class color {

public:
    int red = 0;
    int green = 0;
    int blue = 0;
    
	std::vector<int> to_vector() const;
    void set(int r, int g, int b);
    friend std::ostream& operator<<(std::ostream& os, color const& c);
};


class picture {

public:
    int width;
    int height;
	std::vector<std::vector<color>> grid;

    picture (int width, int height);
	std::vector<color>& operator[](int index);
    friend std::ostream& operator<<(std::ostream& os, picture const& p);

};  


void print(picture& p, std::string s);

#endif

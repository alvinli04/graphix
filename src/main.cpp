#include <fstream>
#include "utils.h"


int main () {
    picture p(750, 750);
    for (int i = 0; i < p.height; i++) {
        for (int j = 0; j < p.width; j++) {
            p[i][j].set(i*j, i+j, abs(i-j));
        }
    }
    
    p.to_ppm("test"); 
}

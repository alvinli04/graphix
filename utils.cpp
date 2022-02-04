#include <iostream>
#include <vector>
#include <cstdio>
#include "utils.h"

using namespace std;

namespace utils {
	
	struct color {
		int red = 0;
		int green = 0;
		int blue = 0;
		
		vector<int> to_vector() const {
			return {red, green, blue};
		}
		
		friend ostream& operator << (ostream &output, const color& c) {
			return (output << c.red << " " << c.green << " " << c.blue);
		}

	};

	class picture {
		private:
			int width;
			int height;
			vector<vector<color>> grid;

		public:
			picture (int width, int height) {
				this->width = width;
				this->height = height;
				grid.resize(height, vector<color> (width));
			}

			void setrgb (int x, int y, int r, int g, int b) {
				//check for out of bounds
				color& c = grid[x][y];
				c.red = r;
				c.green = g;
				c.blue = b;
			}
			
			vector<color>& operator[](int index) {
				//check for out of bounds
				return grid[index];
			}

			friend ostream& operator << (ostream& output, utils::picture& p) {
				output << "P3\n" << p.height << " " << p.width << "\n";
				for (int i = 0; i < p.height; ++i) {
					for (int j = 0; j < p.width; ++j) {
						output << p[i][j] << " ";
					}
					output << "\n";
				}
				return output;
			}
	};	

}

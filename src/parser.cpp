#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "parser.hpp"
#include "matrix.hpp"
#include "draw.hpp"
#include "screen.hpp"

void parse_file (std::string filename, matrix& M, edgelist& E, picture& s) {
	std::ifstream infile;
	infile.open (filename);
	std::cout << "Opened " << filename << std::endl;

	std::string cmd;
	while (getline (infile, cmd)) {
		std::cout << cmd << "\n";
	}

	infile.close ();
}

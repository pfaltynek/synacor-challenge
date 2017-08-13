#include "vm.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
	std::ifstream input;
	std::string line;
	int cnt = 0;
	int value;

	switch (argc) {
		case 0:
		case 1:
			std::cout << "Missing binary file specification for virtual machine" << std::endl;
			return -1;

		case 2:
			input.open(argv[1], std::ifstream::in | std::ifstream::binary);
			if (input.fail()) {
				if (input.is_open()) {
					input.close();
				}
				std::cout << "Error opening input file.\n";
				return -1;
			}
			break;

		default:
			std::cout << "Too many parameters, only binary file name for VM required" << std::endl;
			return -1;
			break;
	}

	input.unsetf(std::ios::skipws);

	cVM vm(input);

	if (input.is_open()) {
		input.close();
	}
}

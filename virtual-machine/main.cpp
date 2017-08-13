#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

int cnt = 0;
short x = 0;

bool get_chalenge_bin_value(std::ifstream &input, short &value) {
	bool result = true;
	unsigned char v[2];

	value = 0;

	for (int i = 0; i < 2; i++) {
		if (input) {
			if (input >> v[i]) {
			} else {
				result = false;
				break;
			}
		} else {
			result = false;
			break;
		}
	}
	if (result) {
		value = v[0] + (v[1] * 256);
	}
	return result;
}

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

	while (get_vm_mem_value(input, value)) {
		cnt += 2;
	}

	cnt += sizeof(x);

	if (input.is_open()) {
		input.close();
	}
}
}
}

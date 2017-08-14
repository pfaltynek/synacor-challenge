#include "vm.h"

bool cVM::get_input_value(std::ifstream &input, short &value) {
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

cVM::cVM(std::ifstream &input) {
	short value;
	_pc = 0;
	for (int i = 0; i < 8; i++) {
		_regs[i] = 0;
	}
	while (!_stack.empty()) {
		_stack.pop();
	}
	_memory.clear();
	_bin_code.clear();

	while (get_input_value(input, value)) {
		_bin_code.push_back(value);
	}
}

TRACE_FINISH_REASON cVM::RunBinCode() {
	TRACE_FINISH_REASON result = TRACE_FINISH_REASON::UNKNOWN;
	bool terminated = false;

	while (_pc < _bin_code.size() && !terminated) {
		switch ((INSTRUCTIONS)_bin_code[_pc]) {
			case INSTRUCTIONS::HALT:
				// stop execution and terminate the program
				result = TRACE_FINISH_REASON::HALTED;
				terminated = true;
				break;
			case INSTRUCTIONS::OUT:
				_pc++;

				_pc++;
				break;
			case INSTRUCTIONS::NOOP:
				_pc++;
				break;
			default:

				break;
		}
	}

	return result;
}

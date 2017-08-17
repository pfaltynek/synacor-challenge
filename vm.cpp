#include "vm.h"
#include <iostream>

#define FIRST_REGISTER_INDEX_VALUE 32768
#define LAST_REGISTER_INDEX_VALUE 32775

bool cVM::get_input_value(std::ifstream &input, unsigned short &value) {
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
	unsigned short value;
	_pc = 0;
	for (int i = 0; i < 8; i++) {
		_regs[i] = 0;
	}
	while (!_stack.empty()) {
		_stack.pop();
	}
	_bin_code.clear();

	while (get_input_value(input, value)) {
		_bin_code.push_back(value);
	}
	while (_bin_code.size() < 32768) {
		_bin_code.push_back(0);
	}
}

bool cVM::IsValueValid(unsigned short value) {
	return (value <= LAST_REGISTER_INDEX_VALUE);
}

bool cVM::IsValueRegister(unsigned short value) {
	return ((value >= FIRST_REGISTER_INDEX_VALUE) && (value <= LAST_REGISTER_INDEX_VALUE));
}

bool cVM::IsValueMemoryAddress(unsigned short value) {
	return (value < FIRST_REGISTER_INDEX_VALUE);
}

bool cVM::GetSourceValue(unsigned short src_value, unsigned short &result) {
	if (IsValueValid(src_value)) {
		if (IsValueRegister(src_value)) {
			result = _regs[src_value - FIRST_REGISTER_INDEX_VALUE];
		} else {
			result = src_value;
		}
		return true;
	} else {
		return false;
	}
}

bool cVM::SetTargetValue(unsigned short storage, unsigned short value) {
	if (IsValueRegister(storage)) {
		_regs[storage - FIRST_REGISTER_INDEX_VALUE] = value;
		return true;
	} else {
		return false;
	}
}

TRACE_FINISH_REASON cVM::RunBinCode() {
	TRACE_FINISH_REASON result = TRACE_FINISH_REASON::UNKNOWN;
	bool terminated = false;
	unsigned short op1, op2, op3, tmp;

	while (_pc < _bin_code.size() && !terminated) {
		switch ((INSTRUCTIONS)_bin_code[_pc]) {
			case INSTRUCTIONS::HALT:
				// stop execution and terminate the program
				result = TRACE_FINISH_REASON::HALTED;
				terminated = true;
				break;
			case INSTRUCTIONS::SET:
				_pc++;
				op1 = _bin_code[_pc];
				_pc++;
				if (GetSourceValue(_bin_code[_pc], op2)) {
					if (SetTargetValue(op1, op2)) {
						_pc++;
					} else {
						result = TRACE_FINISH_REASON::REGISTER_REQUIRED;
						terminated = true;
					}
				} else {
					result = TRACE_FINISH_REASON::INVALID_NUMBER;
					terminated = true;
				}
				break;
			case INSTRUCTIONS::PUSH:
				_pc++;
				if (GetSourceValue(_bin_code[_pc], op1)) {
					_stack.push(op1);
					_pc++;
				} else {
					result = TRACE_FINISH_REASON::INVALID_NUMBER;
					terminated = true;
				}
				break;
			case INSTRUCTIONS::POP:
				if (_stack.empty()) {
					result = TRACE_FINISH_REASON::STACK_EMPTY;
					terminated = true;
					break;
				}
				_pc++;
				op1 = _bin_code[_pc];
				op2 = _stack.top();
				_stack.pop();
				if (SetTargetValue(op1, op2)) {
					_pc++;
				} else {
					result = TRACE_FINISH_REASON::REGISTER_REQUIRED;
					terminated = true;
				}
				break;
			case INSTRUCTIONS::EQ:
				_pc++;
				op1 = _bin_code[_pc];
				_pc++;
				if (GetSourceValue(_bin_code[_pc], op2)) {
					_pc++;
					if (GetSourceValue(_bin_code[_pc], op3)) {
						_pc++;
						tmp = (op2 == op3 ? 1 : 0);
						if (!SetTargetValue(op1, tmp)) {
							result = TRACE_FINISH_REASON::REGISTER_REQUIRED;
							terminated = true;
						}
					} else {
						result = TRACE_FINISH_REASON::INVALID_NUMBER;
						terminated = true;
					}
				} else {
					result = TRACE_FINISH_REASON::INVALID_NUMBER;
					terminated = true;
				}
				break;
			case INSTRUCTIONS::GT:
				_pc++;
				op1 = _bin_code[_pc];
				_pc++;
				if (GetSourceValue(_bin_code[_pc], op2)) {
					_pc++;
					if (GetSourceValue(_bin_code[_pc], op3)) {
						_pc++;
						tmp = (op2 > op3 ? 1 : 0);
						if (!SetTargetValue(op1, tmp)) {
							result = TRACE_FINISH_REASON::REGISTER_REQUIRED;
							terminated = true;
						}
					} else {
						result = TRACE_FINISH_REASON::INVALID_NUMBER;
						terminated = true;
					}
				} else {
					result = TRACE_FINISH_REASON::INVALID_NUMBER;
					terminated = true;
				}
				break;
			case INSTRUCTIONS::JMP:
				_pc++;
				if (GetSourceValue(_bin_code[_pc], op1)) {
					_pc = op1;
				} else {
					result = TRACE_FINISH_REASON::INVALID_NUMBER;
					terminated = true;
				}
				break;
			case INSTRUCTIONS::JT:
				_pc++;
				if (GetSourceValue(_bin_code[_pc], op1)) {
					_pc++;
					if (op1) {
						if (GetSourceValue(_bin_code[_pc], op2)) {
							_pc = op2;
						} else {
							result = TRACE_FINISH_REASON::INVALID_NUMBER;
							terminated = true;
						}
					} else {
						_pc++;
					}
				} else {
					result = TRACE_FINISH_REASON::INVALID_NUMBER;
					terminated = true;
				}
				break;
			case INSTRUCTIONS::JF:
				_pc++;
				if (GetSourceValue(_bin_code[_pc], op1)) {
					_pc++;
					if (!op1) {
						if (GetSourceValue(_bin_code[_pc], op2)) {
							_pc = op2;
						} else {
							result = TRACE_FINISH_REASON::INVALID_NUMBER;
							terminated = true;
						}
					} else {
						_pc++;
					}
				} else {
					result = TRACE_FINISH_REASON::INVALID_NUMBER;
					terminated = true;
				}
				break;
			case INSTRUCTIONS::ADD:
				tmp = 0;
				_pc++;
				op1 = _bin_code[_pc];
				_pc++;
				if (GetSourceValue(_bin_code[_pc], op2)) {
					_pc++;
					if (GetSourceValue(_bin_code[_pc], op3)) {
						_pc++;
						tmp = (op2 + op3) % FIRST_REGISTER_INDEX_VALUE;
						if (!SetTargetValue(op1, tmp)) {
							result = TRACE_FINISH_REASON::REGISTER_REQUIRED;
							terminated = true;
						}
					} else {
						result = TRACE_FINISH_REASON::INVALID_NUMBER;
						terminated = true;
					}
				} else {
					result = TRACE_FINISH_REASON::INVALID_NUMBER;
					terminated = true;
				}
				break;
			case INSTRUCTIONS::MULT:
				tmp = 0;
				_pc++;
				op1 = _bin_code[_pc];
				_pc++;
				if (GetSourceValue(_bin_code[_pc], op2)) {
					_pc++;
					if (GetSourceValue(_bin_code[_pc], op3)) {
						_pc++;
						tmp = (op2 * op3) % FIRST_REGISTER_INDEX_VALUE;
						if (!SetTargetValue(op1, tmp)) {
							result = TRACE_FINISH_REASON::REGISTER_REQUIRED;
							terminated = true;
						}
					} else {
						result = TRACE_FINISH_REASON::INVALID_NUMBER;
						terminated = true;
					}
				} else {
					result = TRACE_FINISH_REASON::INVALID_NUMBER;
					terminated = true;
				}
				break;
			case INSTRUCTIONS::MOD:
				tmp = 0;
				_pc++;
				op1 = _bin_code[_pc];
				_pc++;
				if (GetSourceValue(_bin_code[_pc], op2)) {
					_pc++;
					if (GetSourceValue(_bin_code[_pc], op3)) {
						_pc++;
						tmp = op2 % op3;
						if (!SetTargetValue(op1, tmp)) {
							result = TRACE_FINISH_REASON::REGISTER_REQUIRED;
							terminated = true;
						}
					} else {
						result = TRACE_FINISH_REASON::INVALID_NUMBER;
						terminated = true;
					}
				} else {
					result = TRACE_FINISH_REASON::INVALID_NUMBER;
					terminated = true;
				}
				break;
			case INSTRUCTIONS::AND:
				tmp = 0;
				_pc++;
				op1 = _bin_code[_pc];
				_pc++;
				if (GetSourceValue(_bin_code[_pc], op2)) {
					_pc++;
					if (GetSourceValue(_bin_code[_pc], op3)) {
						_pc++;
						tmp = op2 & op3;
						if (!SetTargetValue(op1, tmp)) {
							result = TRACE_FINISH_REASON::REGISTER_REQUIRED;
							terminated = true;
						}
					} else {
						result = TRACE_FINISH_REASON::INVALID_NUMBER;
						terminated = true;
					}
				} else {
					result = TRACE_FINISH_REASON::INVALID_NUMBER;
					terminated = true;
				}
				break;
			case INSTRUCTIONS::OR:
				tmp = 0;
				_pc++;
				op1 = _bin_code[_pc];
				_pc++;
				if (GetSourceValue(_bin_code[_pc], op2)) {
					_pc++;
					if (GetSourceValue(_bin_code[_pc], op3)) {
						_pc++;
						tmp = op2 | op3;
						if (!SetTargetValue(op1, tmp)) {
							result = TRACE_FINISH_REASON::REGISTER_REQUIRED;
							terminated = true;
						}
					} else {
						result = TRACE_FINISH_REASON::INVALID_NUMBER;
						terminated = true;
					}
				} else {
					result = TRACE_FINISH_REASON::INVALID_NUMBER;
					terminated = true;
				}
				break;
			case INSTRUCTIONS::NOT:
				tmp = 0;
				_pc++;
				op1 = _bin_code[_pc];
				_pc++;
				if (GetSourceValue(_bin_code[_pc], op2)) {
					_pc++;
					tmp = ~op2 & 0x7FFF;
					if (!SetTargetValue(op1, tmp)) {
						result = TRACE_FINISH_REASON::REGISTER_REQUIRED;
						terminated = true;
					}
				} else {
					result = TRACE_FINISH_REASON::INVALID_NUMBER;
					terminated = true;
				}
				break;
			case INSTRUCTIONS::RMEM:
				_pc++;
				op1 = _bin_code[_pc];
				_pc++;
				if (GetSourceValue(_bin_code[_pc], op2)) {
					_pc++;
					if (op2 < _bin_code.size()) {
						tmp = _bin_code[op2];
						if (!SetTargetValue(op1, tmp)) {
							result = TRACE_FINISH_REASON::REGISTER_REQUIRED;
							terminated = true;
						}
					} else {
						result = TRACE_FINISH_REASON::INVALID_MEMORY_ADDRESS;
						terminated = true;
					}
				} else {
					result = TRACE_FINISH_REASON::INVALID_NUMBER;
					terminated = true;
				}
				break;
			case INSTRUCTIONS::WMEM:
				_pc++;
				if (GetSourceValue(_bin_code[_pc], op1)) {
					_pc++;
					if (GetSourceValue(_bin_code[_pc], op2)) {
						if (op2 < _bin_code.size()) {
							_bin_code[op1] = op2;
							_pc++;
						} else {
							result = TRACE_FINISH_REASON::INVALID_MEMORY_ADDRESS;
							terminated = true;
						}
					} else {
						result = TRACE_FINISH_REASON::INVALID_NUMBER;
						terminated = true;
					}
				} else {
					result = TRACE_FINISH_REASON::INVALID_NUMBER;
					terminated = true;
				}
				break;
			case INSTRUCTIONS::CALL:
				_pc++;
				if (GetSourceValue(_bin_code[_pc], op1)) {
					_pc++;
					_stack.push(_pc);
					_pc = op1;
				} else {
					result = TRACE_FINISH_REASON::INVALID_NUMBER;
					terminated = true;
				}
				break;
			case INSTRUCTIONS::RET:
				if (_stack.empty()) {
					result = TRACE_FINISH_REASON::STACK_EMPTY;
					terminated = true;
					break;
				}
				op1 = _stack.top();
				_stack.pop();
				_pc = op1;
				break;
			case INSTRUCTIONS::OUT:
				_pc++;
				if (GetSourceValue(_bin_code[_pc], op1)) {
					std::cout << (char)op1;
					_pc++;
				} else {
					result = TRACE_FINISH_REASON::INVALID_NUMBER;
					terminated = true;
				}
				break;
			case INSTRUCTIONS::IN:

				break;
			case INSTRUCTIONS::NOOP:
				_pc++;
				break;
			default:
				result = TRACE_FINISH_REASON::UNKNOWN_INSTRUCTION;
				std::cout << std::endl
						  << "Unknown instruction: " << _bin_code[_pc] << " at " << _pc << std::endl;
				terminated = true;
				break;
		}
	}

	return result;
}


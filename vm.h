#ifndef VM_H
#define VM_H

#include <fstream>
#include <map>
#include <stack>
#include <vector>

typedef enum {
	UNKNOWN,
	HALTED,
	UNKNOWN_INSTRUCTION,
	INVALID_NUMBER,
	REGISTER_REQUIRED,
	STACK_EMPTY,
} TRACE_FINISH_REASON;

typedef enum {
	HALT = 0,
	SET = 1,
	PUSH = 2,
	POP = 3,
	EQ = 4,
	GT = 5,
	JMP = 6,
	JT = 7,
	JF = 8,
	ADD = 9,
	MULT = 10,
	MOD = 11,
	AND = 12,
	OR = 13,
	NOT = 14,
	// = 15,
	// = 16,
	// = 17,
	// = 18,
	OUT = 19,
	// = 20,
	NOOP = 21
}
INSTRUCTIONS;

class cVM {
  public:
	cVM(std::ifstream &input);
	cVM() = delete;
	cVM(const cVM &) = delete;
	TRACE_FINISH_REASON RunBinCode();

  protected:
	bool get_input_value(std::ifstream &input, unsigned short &value);
	bool IsValueValid(unsigned short value);
	bool IsValueRegister(unsigned short value);
	bool IsValueMemoryAddress(unsigned short value);
	bool GetSourceValue(unsigned short src_value, unsigned short &result);
	bool SetTargetValue(unsigned short storage, unsigned short value);

  private:
	short _pc;
	short _regs[8];
	//std::map<unsigned short, unsigned short> _memory;
	std::stack<unsigned short> _stack;
	std::vector<unsigned short> _bin_code;
};

#endif

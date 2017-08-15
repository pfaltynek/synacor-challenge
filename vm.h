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
	STACK_EMPTY,
} TRACE_FINISH_REASON;

typedef enum {
	HALT = 0,
	JMP = 6,
	JT = 7,
	OUT = 19,
	NOOP = 21
} INSTRUCTIONS;

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

  private:
	short _pc;
	short _regs[8];
	std::map<unsigned short, unsigned short> _memory;
	std::stack<unsigned short> _stack;
	std::vector<unsigned short> _bin_code;
};

#endif

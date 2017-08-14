#ifndef VM_H
#define VM_H

#include <fstream>
#include <map>
#include <stack>
#include <vector>

typedef enum {
	UNKNOWN,
	HALTED,
	STACK_EMPTY
} TRACE_FINISH_REASON;

typedef enum {
	HALT = 0,
	OUT = 19,
	NOOP = 21
} INSTRUCTIONS;

class cVM {
  public:
	cVM(std::ifstream &input);
	TRACE_FINISH_REASON RunBinCode();

  private:
	short _pc;
	short _regs[8];
	std::map<short, short> _memory;
	std::stack<short> _stack;
	std::vector<short> _bin_code;

  protected:
	bool get_input_value(std::ifstream &input, short &value);
};

#endif

#ifndef VM_H
#define VM_H

#include <fstream>
#include <map>
#include <stack>
#include <vector>

class cVM {
  public:
	cVM(std::ifstream &input);
	RunBinCode();

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

#ifndef _INSTRUCTION_H
#define _INSTRUCTION_H
#include <vector>
#include <string>

using namespace std;

typedef int clock_cycle;

struct instruction_status_entry {
	clock_cycle ID1; 
	clock_cycle ID2;
	clock_cycle EX;
	clock_cycle WB;
};

class Instruction {

	public:
	  Instruction();
	  instruction_status_entry status;
	  // type field is going to either be r-format, i-format, j-format, label, nop, or syscall
	  string type;

	  // if the type of the instruction is label, this will be the label value
	  // otherwise this should be empty
	  string label;


	  // label - operands[0] is the offset of the label
	  // r - format: operand[0] = rd, operand[1] = rs, operand[2] = rt/immediate value
	  // i - format: operand[0] = rd, operand[1] = immediate value
	  // j - format: no operands, label to jump to will be stored in the label class member
	  vector<int> operands;
	  vector<float> float_operands;


};

#endif
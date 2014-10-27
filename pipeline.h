#include "instruction.h"
struct if_id_latch {
	Instruction ir;
};

struct id_ex_latch {
	Instruction ir;
	int alu_function;
	int syscall_function;
	string decoded_opcode;
	int offset;
	int immediate;
	int new_PC;
};

struct ex_mem_latch {
	string decoded_opcode;
	int alu_output;
	int rt;
	string rd;
};

struct mem_wb_latch {
	string decoded_opcode;
	int alu_output;
	int rt;
	string rd;
};
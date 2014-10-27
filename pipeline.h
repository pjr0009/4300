#include "instruction.h"
#define EMPTY_LATCH "NULL"

struct if_id_latch {
	Instruction ir;
	string decoded_opcode;
};

struct id_ex_latch {
	int rs;
	int rd;
	int rt;
	int op;
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
	int rd;
};

struct mem_wb_latch {
	string decoded_opcode;
	int alu_output;
	int mdr;
	int operand_b;
	int rd;
};
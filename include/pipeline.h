#include "instruction.h"
#define EMPTY_LATCH "NULL"
#define SKIP_WRITEBACK -1

struct if_id_latch {
	Instruction ir;
	bool empty;
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
	int fu_output;
	int rt;
	int rd;
};

struct mem_wb_latch {
	string decoded_opcode;
	int fu_output;
	unsigned char mdr;
	int operand_b;
	int rd;
};
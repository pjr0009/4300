#include "instruction.h"
#define EMPTY_LATCH "NULL"
#define SKIP_WRITEBACK -1

struct if_id_latch {
	Instruction ir;
	bool empty;
};

// still need this
struct id_ex_latch {
	bool nop; // stall or nah
	int ri;
	int rj;
	int rk;
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
#ifndef _DATA_PATH_H
#define _DATA_PATH_H
#include "memory.h"
#include "register_file.h"
#include "instruction.h"
#include "decoder.h"
class DataPath {
	public:
		bool write_back;
		int alu_function;
		int alu_output;
		vector<Instruction> memory;
		Decoder decoder;
		RegisterFile register_file;
		int alu(int a, int b);
		DataPath(int text_segment_length);

};
#endif
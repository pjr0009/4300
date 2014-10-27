#ifndef _DATA_PATH_H
#define _DATA_PATH_H
#include "memory.h"
#include "register_file.h"
#include "instruction.h"
#include "decoder.h"
class DataPath {
	public:
		bool write_back;
		int alu_output;
		int pc;
		vector<Instruction> memory;
		Decoder decoder;
		RegisterFile register_file;
		int alu(int a, int b, int alu_function);
		DataPath(int text_segment_length);

};
#endif
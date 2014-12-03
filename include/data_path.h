#ifndef _DATA_PATH_H
#define _DATA_PATH_H

#include "memory.h"
#include "register_file.h"
#include "instruction.h"
#include "decoder.h"

using namespace std;

class DataPath {
	public:
		DataPath(int text_segment_length);
		
		bool user_mode;
		int fu_output;
		int pc;
		Decoder decoder;
		vector<Instruction> memory;
		RegisterFile register_file;		
		// map<int, string> data_memory;
		vector<unsigned char> data_segment;

		int memory_write(int addr, string data);
		unsigned char memory_read(int addr);
		int functional_unit(int a, int b, int op);
};

#endif
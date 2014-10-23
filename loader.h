#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "instruction.h"
#include "memory.h"
#include "decoder.h"
#include <map>


using namespace std;

class Loader {
	string file_name;
	ifstream source_file;
	void loader_debug(string instruction, string formatSlot, string token, int binary);
	public:
		Loader(string name);
		int parse_assembly(vector<Instruction>* memory, Decoder decoder);
		int text_segment_length();
		void translate_rformat_to_binary(vector<Instruction>* memory, int j, vector<string> tokens, int length, Decoder decoder);
		void translate_iformat_to_binary(vector<Instruction>* memory, int j, vector<string> tokens, int length, Decoder decoder);
		
		int toBinInt(int val);
		void loader_debug(vector<Instruction>* memory, int index);


};

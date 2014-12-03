#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "instruction.h"
#include "data_path.h"
#include <map>


using namespace std;

class Loader {
	string file_name;
	ifstream source_file;
	void loader_debug(string instruction, string formatSlot, string token, int binary);
	public:
		Loader(string name);
		int parse_assembly(DataPath* data_path);
		int parse_data(DataPath* data_path);

		int text_segment_length();
		int data_segment_length();

		void translate_rformat_to_binary(DataPath* data_path, int j, vector<string> tokens, int length);
		void translate_iformat_to_binary(DataPath* data_path, int j, vector<string> tokens, int length);
		void translate_jformat_to_binary(DataPath* data_path, int j, vector<string> tokens, int length);
		
		int toBinInt(int val);
		void loader_debug(DataPath data_path, int index);
		void memory_debug(DataPath data_path, int index);



};

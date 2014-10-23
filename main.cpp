#include <iostream>
#include "loader.h"
#include <vector>
#include "instruction.h"
#include "decoder.h"
#include "memory.h"
using namespace std;


int main(){

	// get file name
	string file_name;
	cout << "Please enter assembly file name: ";
	cin >> file_name;

	Loader loader(file_name);
	vector<Instruction> memory;
	Decoder decoder;

	int count = loader.text_segment_length();

	// initialize memory with empty instructions
	int i = 0;
	while(i < count){
		memory.push_back(Instruction());
		count--;
	}

	loader.parse_assembly(&memory, decoder);
    



	return 0;
}
#include "data_path.h"


DataPath::DataPath(int text_segment_length){
	
	//initialize memory, pc and usermode
	user_mode = true;
	pc = 0;
	int i = 0;
	while(i < text_segment_length){
		memory.push_back(Instruction());
		text_segment_length--;
	}
};

void DataPath::alu(int a, int b, int alu_function){
	if(alu_function == 1){
		alu_output = a+b;
	}
	else if(alu_function == 2){
		alu_output = a-b;
	}
	else {
		
	}

}
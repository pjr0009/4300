#include "data_path.h"


DataPath::DataPath(int text_segment_length){
	
	//initialize memory 
	pc = 0;
	int i = 0;
	while(i < text_segment_length){
		memory.push_back(Instruction());
		text_segment_length--;
	}
};

int DataPath::alu(int a, int b){
	if(alu_function == 1){
		return a+b;
	}
	else if(alu_function == 2){
		return a-b;
	}
	else {
		return 0;
	}

}
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

int DataPath::alu(int a, int b, int op){
	if(op == 1){
		return a+b;
	}
	else if(op == 2){
		return a-b;
	}
	else if(op == 3){
		cout << endl << "EXECUTING BEQZ WITH A = " << a << " & B = " << b << endl;
		if(a==b){
			return 1;
		} else {
			return 0;
		}
	}
	else{
		return 0;
	}

}
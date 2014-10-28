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

int DataPath::memory_write(int addr, string data){
	data_memory.insert(std::pair<int, string>(addr, data));
	return 0;
}

string DataPath::memory_read(int addr)
{
	string out;
	try {
		out = data_memory.at(addr);
	}
	catch (int e){
		cout << "INVALID MEMORY ACCESS" << endl;
	}

	return out;
}

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
			return 0;
		} else {
			return 1;
		}
	}
	else if(op == 4){
		cout << "EXECUTIN A BGE WITH A = " << a << " & B = " << b << endl;
		if (a >= b)
			return 0;
		else
			return 1;
	}
	else if (op == 5){
		cout << "EXECUTING A BNE WITH A = " << a << " & B = " << b << endl;
		if (a != b)
			return 0;
		else
			return 1;
	}
	else{
		return 0;
	}

}
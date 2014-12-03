#include "../include/data_path.h"


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
	cout << "WRITING " << data << "TO ADDR" << addr << endl;
	// +1 to these calculations to include the null terminator at the end of data
	if (data_segment.size() < addr + data.size() + 1) {
		data_segment.resize(addr + data.size() + 1);
	}
	memcpy(data_segment.data() + addr, data.c_str(), data.size() + 1);
	return 0;
}

unsigned char DataPath::memory_read(int addr)
{
	return data_segment.at(addr);
}

int DataPath::functional_unit(int a, int b, int op){
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
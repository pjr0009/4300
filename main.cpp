#include <iostream>
#include "loader.h"
#include <vector>
#include "data_path.h"
#include "stages.h"
using namespace std;


int main(){

	// get file name
	string file_name;
	cout << "Please enter assembly file name: ";
	cin >> file_name;

	Loader loader(file_name);

	int count = loader.text_segment_length();

	DataPath data_path(count);

	cout << endl << "/******** ***************************** ********/" << endl;
	cout << "/******** LOAD INSTRUCTIONS INTO MEMORY ********/" << endl;
	cout << "/******** ***************************** ********/" << endl;

	loader.parse_assembly(&data_path);

    if_id_latch if_id;
    id_ex_latch id_ex;
    ex_mem_latch ex_mem;
    mem_wb_latch mem_wb;

    while(data_path.pc < data_path.memory.size()){

    	// instruction fetch
    	if_stage(&data_path, &if_id);
    	

    	// instruction decode
    	id_stage(&data_path, &if_id, &id_ex);

    	// execute 
    	execute_stage(&data_path, &id_ex, &ex_mem);

        // till memory_stage is written
        mem_wb.decoded_opcode = ex_mem.decoded_opcode;
        mem_wb.alu_output = ex_mem.alu_output;
        mem_wb.rt = ex_mem.rt;
        mem_wb.rd = ex_mem.rd;

    	//write back
    	wb_stage(&data_path, &mem_wb);

    	// execute();
    }


	return 0;
}
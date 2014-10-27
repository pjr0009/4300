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

    if_id_latch old_if_id;
    old_if_id.decoded_opcode = EMPTY_LATCH;

    id_ex_latch old_id_ex;
    old_id_ex.decoded_opcode = EMPTY_LATCH;

    ex_mem_latch old_ex_mem;
    old_ex_mem.decoded_opcode = EMPTY_LATCH;

    mem_wb_latch old_mem_wb;
    old_mem_wb.decoded_opcode = EMPTY_LATCH;

    while(data_path.pc < data_path.memory.size() && data_path.user_mode){

        mem_wb_latch new_mem_wb;
        ex_mem_latch new_ex_mem;
        id_ex_latch new_id_ex;
        if_id_latch new_if_id;

    	// instruction fetch
    	if_stage(&data_path, &new_if_id);
    	// instruction decode
    	id_stage(&data_path, &old_if_id, &new_id_ex);
        old_if_id = new_if_id;

    	// execute 
    	execute_stage(&data_path, &old_id_ex, &new_ex_mem);
        old_id_ex = new_id_ex;

        // till memory_stage is written
        memory_stage(&data_path, &old_ex_mem, &new_mem_wb);
        old_ex_mem = new_ex_mem;

    	//write back
    	wb_stage(&data_path, &old_mem_wb);
        old_mem_wb = new_mem_wb;
    }


	return 0;
}
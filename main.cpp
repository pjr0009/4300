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
    old_if_id.empty = true;

    id_ex_latch old_id_ex;
    old_id_ex.decoded_opcode = EMPTY_LATCH;

    ex_mem_latch old_ex_mem;
    old_ex_mem.decoded_opcode = EMPTY_LATCH;

    mem_wb_latch old_mem_wb;
    old_mem_wb.decoded_opcode = EMPTY_LATCH;
    int cycle = 0;
    //increment upon entering, decrement when leaving
    int in_pipeline = 0;

    while(data_path.pc < data_path.memory.size() && data_path.user_mode){
        in_pipeline ++;
        mem_wb_latch new_mem_wb;
        ex_mem_latch new_ex_mem;
        id_ex_latch new_id_ex;
        if_id_latch new_if_id;

        //data_path.memory_write(12, "HELLO WORLD");
        //cout << data_path.memory_read(12) << endl;

        cout << "STARTING IF_STAGE CYCLE " << cycle << endl;
    	// instruction fetch
    	if_stage(&data_path, &new_if_id);

        cout << "STARTING ID_STAGE CYCLE " << cycle << endl;
    	// instruction decode
    	id_stage(&data_path, &old_if_id, &new_id_ex);
        old_if_id = new_if_id;

        cout << "STARTING EX_STAGE CYCLE " << cycle << endl;
    	// execute 
    	execute_stage(&data_path, &old_id_ex, &new_ex_mem);
        old_id_ex = new_id_ex;

        cout << "STARTING MEM_STAGE CYCLE " << cycle << endl;
        // till memory_stage is written
        memory_stage(&data_path, &old_ex_mem, &new_mem_wb);
        old_ex_mem = new_ex_mem;

        cout << "STARTING WB_STAGE CYCLE " << cycle << "FOR INSTRUCTION: " << old_mem_wb.decoded_opcode << endl;
    	//write back
    	wb_stage(&data_path, &old_mem_wb, &in_pipeline);
        old_mem_wb = new_mem_wb;
        cycle++;
        cout << endl;
    }

    //flush pipeline
    while(in_pipeline > 0){
        mem_wb_latch new_mem_wb;
        ex_mem_latch new_ex_mem;
        id_ex_latch new_id_ex;
        if_id_latch new_if_id;

        switch(in_pipeline){
          //last instruction in pipeline
          case 1:
            old_if_id.empty = true;
            old_id_ex.decoded_opcode = EMPTY_LATCH;
            old_ex_mem.decoded_opcode = EMPTY_LATCH;

            cout << "STARTING WB_STAGE CYCLE " << cycle << "FOR INSTRUCTION: " << old_mem_wb.decoded_opcode << endl;
            //write back
            wb_stage(&data_path, &old_mem_wb, &in_pipeline);
            old_mem_wb = new_mem_wb;
          // 2 instructions still in pipeline
          case 2:
            old_if_id.empty = true;
            old_id_ex.decoded_opcode = EMPTY_LATCH;
            cout << "STARTING MEM_STAGE CYCLE " << cycle << endl;
            // till memory_stage is written
            memory_stage(&data_path, &old_ex_mem, &new_mem_wb);
            old_ex_mem = new_ex_mem;

            cout << "STARTING WB_STAGE CYCLE " << cycle << "FOR INSTRUCTION: " << old_mem_wb.decoded_opcode << endl;
            //write back
            wb_stage(&data_path, &old_mem_wb, &in_pipeline);
            old_mem_wb = new_mem_wb;
            cycle++;
          // 3 instructions in pipeline
          case 3:
            old_if_id.empty = true;
            
            cout << "STARTING EX_STAGE CYCLE " << cycle << endl;
            // execute 
            execute_stage(&data_path, &old_id_ex, &new_ex_mem);
            old_id_ex = new_id_ex;

            cout << "STARTING MEM_STAGE CYCLE " << cycle << endl;
            // till memory_stage is written
            memory_stage(&data_path, &old_ex_mem, &new_mem_wb);
            old_ex_mem = new_ex_mem;

            cout << "STARTING WB_STAGE CYCLE " << cycle << "FOR INSTRUCTION: " << old_mem_wb.decoded_opcode << endl;
            //write back
            wb_stage(&data_path, &old_mem_wb, &in_pipeline);
            old_mem_wb = new_mem_wb;
            cycle++;

          case 4:
            cout << "STARTING ID_STAGE CYCLE " << cycle << endl;
            // instruction decode
            id_stage(&data_path, &old_if_id, &new_id_ex);
            old_if_id = new_if_id;

            cout << "STARTING EX_STAGE CYCLE " << cycle << endl;
            // execute 
            execute_stage(&data_path, &old_id_ex, &new_ex_mem);
            old_id_ex = new_id_ex;

            cout << "STARTING MEM_STAGE CYCLE " << cycle << endl;
            // till memory_stage is written
            memory_stage(&data_path, &old_ex_mem, &new_mem_wb);
            old_ex_mem = new_ex_mem;

            cout << "STARTING WB_STAGE CYCLE " << cycle << "FOR INSTRUCTION: " << old_mem_wb.decoded_opcode << endl;
            //write back
            wb_stage(&data_path, &old_mem_wb, &in_pipeline);
            old_mem_wb = new_mem_wb;
            cycle++;

            }
            cycle++;

    }


	return 0;
}
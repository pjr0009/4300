#include <iostream>
#include "../include/loader.h"
#include <vector>
#include "../include/data_path.h"
#include "../include/stages.h"
using namespace std;


int main(){

	// get file name
	string file_name;
	cout << "Please enter assembly file name: ";
	cin >> file_name;

	Loader loader(file_name);

	int count = loader.text_segment_length();
    int data_segment_length = loader.data_segment_length();
    cout << count << endl;

    // construct data path of appropriate length
	DataPath data_path(count);

	cout << endl << "/******** ***************************** ********/" << endl;
	cout << "/******** LOAD INSTRUCTIONS INTO MEMORY ********/" << endl;
	cout << "/******** ***************************** ********/" << endl;

	loader.parse_assembly(&data_path);
    loader.parse_data(&data_path);

   
    int cycle = 1;
    //increment upon entering, decrement when leaving
    int in_pipeline = 0;

    while(data_path.pc < data_path.memory.size() && data_path.user_mode){
        in_pipeline ++;


        //data_path.memory_write(12, "HELLO WORLD");
        //cout << data_path.memory_read(12) << endl;
    	
        // instruction fetch / issue to reservation station (program order)
    	id1_stage(&data_path);

    	// instruction decode reservation station (out of order possible)
    	id2_stage(&data_path);

        // execute (out of order possible)
        execute_stage(&data_path);
    	
        //write result
    	wb_stage(&data_path, &in_pipeline);

     //    old_mem_wb = new_mem_wb;

        // cout << "***************** LATCHES AFTER CYCLE " << cycle << "************************" << endl;
        // cout << "IF_ID LATCH: " << endl;
        // cout << "ir opcode: " << data_path.decoder.opcodeDecode[old_if_id.ir.operands[0]] << endl;
        // cout << "operand 1: " <<old_if_id.ir.operands[1] << endl;
        // cout << "operand 2: " << old_if_id.ir.operands[2] << endl;


        // cout << endl;
        // cout << "ID_EX LATCH: " << endl;
        // cout << "decoded_opcode: " << old_id_ex.decoded_opcode << endl;
        // cout << "rs: " << old_id_ex.rs << endl;
        // cout << "rt: " << old_id_ex.rt << endl;
        // cout << "rd: " << old_id_ex.rd << endl;
        // cout << "op: " << old_id_ex.op << endl;
        // cout << "syscall_function: " << old_id_ex.syscall_function << endl;
        // cout << "new_PC: " << old_id_ex.new_PC << endl;
        // cout << endl;
        // cout << "EX_MEM LATCH: " << endl;
        // cout << "rd: " << old_ex_mem.rd << endl;
        // cout << "rt: " << old_ex_mem.rt << endl;

        // cout << "************************************************************************" << endl;


        cycle++;
        cout << endl;
    }

    cout << endl << endl << "FLUSHING PIPELINE" << endl << endl;

    //flush pipeline
    while(in_pipeline > 0){
        // mem_wb_latch new_mem_wb;
        // ex_mem_latch new_ex_mem;
        // id_ex_latch new_id_ex;
        // if_id_latch new_if_id;

        // switch(in_pipeline){
        //   //last instruction in pipeline
        //   case 1:
        //     cout << "STARTING WB_STAGE CYCLE " << cycle << " FOR INSTRUCTION: " << old_mem_wb.decoded_opcode << endl;
        //     wb_stage(&data_path, &old_mem_wb, &in_pipeline);
            
        //     cycle++;
        //     cout <<  "remaining instructions in pipeline: " << in_pipeline << endl << endl;
        //     break;

        //   // 2 instructions still in pipeline
        //   case 2:
        //     cout << "STARTING MEM_STAGE CYCLE " << cycle << endl;
        //     memory_stage(&data_path, &old_ex_mem, &new_mem_wb);

        //     cout << "STARTING WB_STAGE CYCLE " << cycle << " FOR INSTRUCTION: " << old_mem_wb.decoded_opcode << endl;
        //     wb_stage(&data_path, &old_mem_wb, &in_pipeline);
        //     old_mem_wb = new_mem_wb;
            
        //     cycle++;
        //     cout <<  "remaining instructions in pipeline: " << in_pipeline << endl << endl;
        //     break;

        //   // 3 instructions in pipeline
        //   case 3:            
        //     cout << "STARTING EX_STAGE CYCLE " << cycle << endl;
        //     execute_stage(&data_path, &old_id_ex, &new_ex_mem);

        //     cout << "STARTING MEM_STAGE CYCLE " << cycle << endl;
        //     memory_stage(&data_path, &old_ex_mem, &new_mem_wb);
        //     old_ex_mem = new_ex_mem;

        //     cout << "STARTING WB_STAGE CYCLE " << cycle << " FOR INSTRUCTION: " << old_mem_wb.decoded_opcode << endl;
        //     wb_stage(&data_path, &old_mem_wb, &in_pipeline);
        //     old_mem_wb = new_mem_wb;
            
        //     cycle++;
        //     cout <<  "remaining instructions in pipeline: " << in_pipeline << endl << endl;
        //     break;

        //   case 4:
        //     cout << "STARTING ID_STAGE CYCLE " << cycle << endl;
        //     id_stage(&data_path, &old_if_id, &new_id_ex);
        //     old_if_id = new_if_id;

        //     cout << "STARTING EX_STAGE CYCLE " << cycle << endl;
        //     execute_stage(&data_path, &old_id_ex, &new_ex_mem);
        //     old_id_ex = new_id_ex;

        //     cout << "STARTING MEM_STAGE CYCLE " << cycle << endl;
        //     memory_stage(&data_path, &old_ex_mem, &new_mem_wb);
        //     old_ex_mem = new_ex_mem;

        //     cout << "STARTING WB_STAGE CYCLE " << cycle << " FOR INSTRUCTION: " << old_mem_wb.decoded_opcode << endl;
        //     wb_stage(&data_path, &old_mem_wb, &in_pipeline);
        //     old_mem_wb = new_mem_wb;
            
        //     cycle++;
        //     cout <<  "remaining instructions in pipeline: " << in_pipeline << endl << endl;
        //     break;
        //     }
    }

	return 0;
}
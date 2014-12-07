#include <iostream>
#include "../include/loader.h"
#include <vector>
#include "../include/data_path.h"
#include "../include/stages.h"
#include "../include/scoreboard.h"

using namespace std;


int main(){

    id_ex_latch new_id_ex;
    id_ex_latch old_id_ex; // only used if pipelining



	// get file name
	string file_name;
	cout << "Please enter assembly file name: ";
	cin >> file_name;
    int pipeline;
    // cout << "Pipeline decode/ex stage? (0/1): ";
    // cin >> pipeline;

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

    Scoreboard scobo(count, data_path);

   
    int cycle = 1;

        while(data_path.pc < data_path.memory.size() && data_path.user_mode){

            //data_path.memory_write(12, "HELLO WORLD");
            //cout << data_path.memory_read(12) << endl;
        	
            // instruction fetch / issue to reservation station (program order)
            cout << "ENTERING ID1" << endl;
        	id1_stage(&data_path, &scobo, &cycle);

        	// instruction decode reservation station (out of order possible)
        	cout << "ENTERING ID2" << endl;
            id2_stage(&data_path, &scobo, &cycle);

            cout << "ENTERING EXE" << endl;
            execute_stage(&data_path, &scobo, &cycle);

            cout << "ENTERING WB" << endl;
            writeback_stage(&data_path, &scobo, &cycle);
            
            // print our scoreboard
            scobo.debug(&data_path);
            scobo.invalidate_dirty_bits();




            // execute (out of order possible)
            // execute_stage(&data_path);
        	
            //write result
        	// wb_stage(&data_path, &in_pipeline);

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

    cout << endl << endl << "SIMULATION COMPLETE" << endl << endl;

	return -1;
}
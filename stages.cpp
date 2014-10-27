#include "stages.h"

void if_stage(DataPath *data_path, if_id_latch *if_id){
	Instruction current_instruction = data_path -> memory.at(data_path -> pc);
	// label?
	while(data_path -> pc < data_path -> memory.size() && data_path -> memory.at(data_path -> pc).type == "label"){
		data_path -> pc += 1;
		current_instruction = data_path -> memory.at(data_path -> pc);
	}
	// put instruction at program counter into instruction register
	data_path -> register_file.instruction_register = current_instruction;
	// add current instruction the the if_id_latch
	if_id -> ir = current_instruction;
	// increment pc
	data_path -> pc = (data_path -> pc) + 1;
	if_debug(*data_path, if_id);
}

// Read registers rs and rt in case we need them
// Compute the branch address in case the instruction is a branch
void id_stage(DataPath *data_path, if_id_latch *if_id, id_ex_latch *id_ex){
	string opcode = data_path -> decoder.opcodeDecode[if_id -> ir.operands[0]];
	id_ex -> decoded_opcode = opcode;
	if( opcode == "addi" || opcode == "subi" || opcode == "add"){
		// set alu function code
		if(opcode == "addi" || opcode == "add"){
			id_ex -> op = 1;
		} else if (opcode == "subi") {
			id_ex -> op = 2;
		}
		
		id_ex->rd = if_id -> ir.operands[1];
		// decode rs
		int rs = if_id->ir.operands[2];
		int rt = if_id->ir.operands[3];
		//decode rs and set to id_ex rs
		id_ex->rs = data_path -> register_file.registers[data_path -> decoder.registerDecode[rs]];
		
		if(opcode == "add"){
			//decode 3rd register
			id_ex->rt = data_path -> register_file.registers[data_path -> decoder.registerDecode[rt]];
		} else {
			id_ex->rt = rt;
		}



		id_debug(*data_path, id_ex);
		

	} else if(opcode == "b"){
		id_ex -> op = 0; // nop
		int last = data_path -> memory.size() - 1;
	    for(int i = 1; i < last; ++i) {
	    	if(data_path -> memory.at(i).type == "label"){
	    		if(if_id->ir.label == data_path -> memory.at(i).label){
	    			//update PC (we already set op to 0 so nothing else needs to be done here)
	    			data_path -> pc = i;
	    		}
	    	}
	    }

	} else if (opcode == "lb"){
		id_ex -> op = 0;
		id_ex->rd = if_id -> ir.operands[1];
		int rt = if_id->ir.operands[2];
		// substitue value of register reference with actual register value before heading into execution/write back
		// from that point we execute as if it were an li instruction
		id_ex -> rt = data_path -> register_file.registers[data_path -> decoder.registerDecode[rt]];
		data_path -> write_back = true;

	} else if (opcode == "li") {
		id_ex -> op = 0;
		id_ex->rd = if_id -> ir.operands[1];
		id_ex->rt = if_id -> ir.operands[2];
		data_path -> write_back = true;
	} else if (opcode == "syscall"){
		id_ex -> op = 0; //nop
		// put $v0 into id_ex.syscall_function
		id_ex -> syscall_function = data_path -> register_file.registers["$2"];
	}


}
void execute_stage(DataPath *data_path, id_ex_latch *id_ex, ex_mem_latch *ex_mem){
	string opcode = id_ex -> decoded_opcode;
	//unless nop
	if(id_ex -> op > 0){
		if(opcode == "addi" || opcode == "subi" || opcode == "add"){
			ex_mem -> alu_output = data_path -> alu(id_ex -> rs, id_ex -> rt, id_ex -> op);	
			data_path -> write_back = true;
		}
		else if (opcode == "syscall"){
			if(id_ex -> syscall_function == 10){
				printf("syscall function 10 called, need to implement what it actually does\n");
			} else{
				cout << "sycall function" << id_ex -> syscall_function << endl;
			}
		}
	}
	ex_mem -> decoded_opcode = opcode;
	ex_mem -> rd = id_ex -> rd;
	ex_mem -> rt = id_ex -> rt;

}


void memory_stage(DataPath *data_path, ex_mem_latch *ex_mem, mem_wb_latch *mem_wb){
		string opcode = ex_mem ->decoded_opcode;
		// nop if instruction isnt lb *may need to add other instructions here if there are others that use memory*
		if(opcode != "lb"){
	    	mem_wb -> decoded_opcode = ex_mem -> decoded_opcode;
        	mem_wb -> alu_output = ex_mem -> alu_output;
       		mem_wb -> operand_b = ex_mem -> rt;
        	mem_wb -> rd = ex_mem -> rd;

		}

}


void wb_stage (DataPath *data_path, mem_wb_latch *mem_wb){
	if(data_path -> write_back){
		// load immediate, value already available in instruction
		string opcode = mem_wb -> decoded_opcode;
		string dest_reg = data_path -> decoder.registerDecode[mem_wb -> rd];

		if(opcode == "li"){
			data_path -> register_file.registers[dest_reg] = mem_wb -> operand_b;
		}
		if(opcode == "addi" || opcode == "subi" || opcode == "add"){
			data_path -> register_file.registers[dest_reg] = mem_wb -> alu_output;
		}
		// load from data, value shoudl be in mdr
		wb_debug(*data_path, mem_wb);

	}
	data_path -> write_back = false;
}


void if_debug(DataPath data_path, if_id_latch *if_id){
	printf("\nIF STAGE - INSTRUCTION FETCHED: %s \n", data_path.decoder.opcodeDecode[if_id->ir.operands[0]].c_str());
}

void id_debug(DataPath data_path, id_ex_latch *id_ex){
	string opcode = id_ex->decoded_opcode;
	if(opcode == "add" || opcode == "addi" || opcode == "li" || opcode == "lb" || opcode == "subi"){
		printf("\nID STAGE - INSTUCTION DECODED: %s, ALU OP: %d \n", id_ex -> decoded_opcode.c_str(), id_ex -> op);
	}
}

void wb_debug(DataPath data_path, mem_wb_latch *mem_wb){
	string dest_reg = data_path.decoder.registerDecode[mem_wb -> rd];
	cout << endl << "WB STAGE: wrote - " << data_path.register_file.registers[dest_reg] << " to register - " << dest_reg << endl;
}
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
	string opcode;
	if(if_id -> ir.type == "syscall"){
		opcode = "syscall";

	} else {
		opcode = data_path -> decoder.opcodeDecode[if_id -> ir.operands[0]];
	}
	id_ex -> decoded_opcode = opcode;
	if( opcode == "addi" || opcode == "subi"){
		// set alu function code
		if(opcode == "addi"){
			id_ex -> alu_function = 1;
		} else {
			id_ex -> alu_function = 2;
		}
		// decode rs

		int rs = if_id->ir.operands[2];
		int rt = if_id->ir.operands[3];

		if_id->ir.operands[2] = data_path -> register_file.registers[data_path -> decoder.registerDecode[rs]];

		id_debug(*data_path, id_ex);
		

	} else if(opcode == "b"){
		int last = data_path -> memory.size() - 1;
	    for(int i = 1; i < last; ++i) {
	    	if(data_path -> memory.at(i).type == "label"){
	    		if(if_id->ir.label == data_path -> memory.at(i).label){
	    			//set label operand in instruction to actual offset
	    			if_id->ir.operands[1] = i;
	    			data_path -> pc = i;
	    		}
	    	}
	    }

	} else if (opcode == "lb"){
		int rs = if_id->ir.operands[2];

		// substitue value of register reference with actual register value before heading into execution/write back
		// from that point we execute as if it were an li instruction
		id_ex -> ir.operands[2] = data_path -> register_file.registers[data_path -> decoder.registerDecode[rs]];
		data_path -> write_back = true;

	} else if (opcode == "li") {
		data_path -> write_back = true;
	} else if (opcode == "syscall"){
		// put $v0 into id_ex.syscall_function
		id_ex -> syscall_function = data_path -> register_file.registers["$2"];
	}

	id_ex -> ir = if_id -> ir;

}
void execute_stage(DataPath *data_path, id_ex_latch *id_ex, ex_mem_latch *ex_mem){
	string opcode = id_ex -> decoded_opcode;
	if(opcode == "addi"){
		data_path -> alu(id_ex -> ir.operands[2], id_ex -> ir.operands[3], id_ex -> alu_function);	
		ex_mem -> alu_output = data_path -> alu_output;		
		data_path -> write_back = true;
	}
	else if (opcode == "subi"){
		data_path -> alu(id_ex -> ir.operands[2], id_ex -> ir.operands[3], id_ex -> alu_function);	
		ex_mem -> alu_output = data_path -> alu_output;
		data_path -> write_back = true;	
	}
	else if (opcode == "syscall"){
		if(id_ex -> syscall_function == 10){
			printf("syscall function 10 called, need to implement what it actually does\n");
		} else{
			cout << "sycall function" << id_ex -> syscall_function << endl;
		}
	}
	ex_mem -> decoded_opcode = opcode;
	ex_mem -> rd = data_path -> decoder.registerDecode[id_ex -> ir.operands[1]];
	ex_mem -> rt = id_ex -> ir.operands[2];
}




void wb_stage (DataPath *data_path, mem_wb_latch *mem_wb){
	if(data_path -> write_back){
		// load immediate, value already available in instruction

		string opcode = mem_wb -> decoded_opcode;
		string dest_reg = mem_wb -> rd;
		if(opcode == "li"){
			data_path -> register_file.registers[dest_reg] = mem_wb -> rt;
			cout << endl << "wrote: " << data_path -> register_file.registers[dest_reg] << " to register" << dest_reg << endl;
		}
		if(opcode == "addi" || opcode == "subi"){
			data_path -> register_file.registers[dest_reg] = mem_wb -> alu_output;
			cout << endl << "wrote: " << data_path -> register_file.registers[dest_reg] << " to register" << dest_reg << endl;
		}
		// load from data, value shoudl be in mdr

	}
}


void if_debug(DataPath data_path, if_id_latch *if_id){
	printf("loaded instruction: %s \n", data_path.decoder.opcodeDecode[if_id->ir.operands[0]].c_str());
}

void id_debug(DataPath data_path, id_ex_latch *id_ex){
	if(id_ex->ir.type == "r-format")
	printf("decoded instruction: %s \n", data_path.decoder.opcodeDecode[data_path.register_file.instruction_register.operands[0]].c_str());
}
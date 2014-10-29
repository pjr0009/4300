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
	if_id -> empty = false;

	// increment pc
	if_debug(*data_path, if_id);
	data_path -> pc = (data_path -> pc) + 1;

}

void id_stage(DataPath *data_path, if_id_latch *if_id, id_ex_latch *id_ex){
	if(if_id->empty) {
		id_ex->decoded_opcode = EMPTY_LATCH;
		//No operation to be performed
		return;
	}
	else if (if_id->ir.type == "nop") {
		id_ex->decoded_opcode = "nop";
		id_ex->rd =0;
		cout << "ID STAGE: NOP ENCOUNTERED" << endl;
		return;
	}

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



		//id_debug(*data_path, id_ex);
		

	} else if(opcode == "b"){
		id_ex -> op = 0; // nop
		int last = data_path -> memory.size() - 1;
	    for(int i = 1; i < last; ++i) {
	    	if(data_path -> memory.at(i).type == "label"){
	    		if(if_id->ir.label == data_path -> memory.at(i).label){
	    			//update PC (we already set op to 0 so nothing else needs to be done here)
	    			data_path -> pc = i;
	    			cout << "BRANCH TAKEN! NEW PC: " << id_ex -> new_PC << endl;

	    		}
	    	}
	    }
	} else if(opcode == "beqz"){
		id_ex -> op = 3;
		// decode rs (the operand that we check for zero)
		id_ex -> rs = data_path -> register_file.registers[data_path -> decoder.registerDecode[if_id -> ir.operands[1]]];
		int last = data_path -> memory.size() - 1;
	    for(int i = 1; i < last; ++i) {
	    	if(data_path -> memory.at(i).type == "label"){
	    		if(if_id->ir.label == data_path -> memory.at(i).label){
	    			//update PC (we already set op to 0 so nothing else needs to be done here)
	    			id_ex -> new_PC = i;

	    			cout << "SET BRANCH TAKEN PC TO: " << id_ex -> new_PC << " VALUE FOR COMPARISON: " << id_ex -> rs << " READ FROM REGISTER: " << if_id -> ir.operands[1]  << endl;
	    			break;
	    		}
	    	}
	    }
	    // pass along values rs(comparator value), and rt(branch taken address)
		int rs = if_id->ir.operands[1];
		int rt = if_id->ir.operands[2];
		id_ex ->rd = 0;
		id_ex->rs = data_path -> register_file.registers[data_path -> decoder.registerDecode[rs]];
		id_ex->rt = data_path -> register_file.registers[data_path -> decoder.registerDecode[rt]];

	} else if(opcode == "bge"){
		id_ex -> op = 4;
		int last = data_path -> memory.size() - 1;
		for(int i = 1; i < last; ++i) {
			if(data_path -> memory.at(i).type == "label"){
				if(if_id->ir.label == data_path -> memory.at(i).label){
					id_ex -> new_PC = i;
					break;
				}
			}
		}
		int rs = if_id->ir.operands[1];
		int rt = if_id->ir.operands[2];
		id_ex->rs = data_path -> register_file.registers[data_path -> decoder.registerDecode[rs]];
		id_ex->rt = data_path -> register_file.registers[data_path -> decoder.registerDecode[rt]];
	}else if (opcode == "bne"){
		id_ex -> op = 5;
		int last = data_path -> memory.size() - 1;
		for(int i = 1; i < last; ++i) {
			if(data_path -> memory.at(i).type == "label"){
				if (if_id->ir.label == data_path -> memory.at(i).label){
					id_ex -> new_PC = i;
					break;
				}
			}
		}
		int rs = if_id->ir.operands[1];
		int rt = if_id->ir.operands[2];
		id_ex->rs = data_path -> register_file.registers[data_path -> decoder.registerDecode[rs]];
		id_ex->rt = data_path -> register_file.registers[data_path -> decoder.registerDecode[rt]];
	}else if (opcode == "lb"){
		id_ex -> op = 0;
		id_ex->rd = if_id -> ir.operands[1];
		int rt = if_id->ir.operands[2];
		// substitue value of register reference with actual register value before heading into execution/write back
		// from that point we execute as if it were an li instruction
		id_ex -> rt = data_path -> register_file.registers[data_path -> decoder.registerDecode[rt]];
	} else if (opcode == "li" || opcode == "la") {
		id_ex -> op = 0;
		id_ex->rd = if_id -> ir.operands[1];
		id_ex->rt = if_id -> ir.operands[2];
	} else if (opcode == "syscall"){
		id_ex -> op = 0; //nop
		// put $v0 into id_ex.syscall_function
		id_ex -> syscall_function = data_path -> register_file.registers["$2"];
		cout << "LOADED SYSCALL FUNCTION NUMBER: " << id_ex -> syscall_function << endl;
 	}
	id_debug(*data_path, id_ex);

}
void execute_stage(DataPath *data_path, id_ex_latch *id_ex, ex_mem_latch *ex_mem){
	string opcode = id_ex -> decoded_opcode;
	ex_mem -> decoded_opcode = opcode;
	if( opcode == EMPTY_LATCH) {
		//No operation to be performed
		return;
	}
	else if (opcode == "nop") {
		ex_mem->decoded_opcode = "nop";
		cout << "EXE STAGE: NOP ENCOUNTERED" << endl;
		return;
	}
	//unless nop
	if(id_ex -> op > 0 || id_ex -> syscall_function > 0){
		if(opcode == "addi" || opcode == "subi" || opcode == "add"){
			cout << "EXECUTING: " << opcode << endl;
			ex_mem -> alu_output = data_path -> alu(id_ex -> rs, id_ex -> rt, id_ex -> op);	
		}
		else if (opcode == "syscall"){
			if(id_ex -> syscall_function == 10){
				printf("SYSCALL EXIT CALLED: EXITING...\n");
				data_path -> user_mode = false;
			} else if (id_ex -> syscall_function == 8){
				string input;
				cout << "Please enter a word" << endl;
				cin >> input;
				data_path -> memory_write(data_path -> register_file.registers["$4"], input);
				cout << "WROTE: " << input << " TO MEMORY OFFSET " << data_path -> register_file.registers["$4"] << endl;
			} 
			 else if (id_ex -> syscall_function == 4){
				int addr = data_path -> register_file.registers["$4"];
				cout << endl << "STARTING OFFSET: " << addr << endl;
				cout << endl << endl << "SYSCALL PRINT MESSAGE: ";
				char byte;
				for(int i =addr; i < data_path->data_segment.size(); i++){
					if(data_path->memory_read(i) != '\0'){
						cout << data_path -> memory_read(i);
					} else {
						break;
					}
				}
                cout << endl;
                exit(1);
			} 
			else{
				printf("UNKNOWN SYSCALL\n");
			}
		} else if (opcode == "beqz"){
			ex_mem -> alu_output = data_path -> alu(id_ex -> rs, 0, id_ex -> op);
			if(ex_mem -> alu_output == 0){
				cout << "BRANCH TAKEN, NEW PC  " << id_ex -> new_PC << endl;
				data_path -> pc = id_ex -> new_PC;
			}
		} else if (opcode == "bge"){
			ex_mem -> alu_output = data_path -> alu(id_ex -> rs, id_ex -> rt, id_ex -> op);
			if(ex_mem -> alu_output == 0){
				cout << "BRANCH TAKEN" << endl;
				data_path -> pc = id_ex -> new_PC;
			}
		} else if (opcode == "bne"){
			ex_mem -> alu_output = data_path -> alu(id_ex -> rs, id_ex -> rt, id_ex -> op);
			if (ex_mem -> alu_output == 0){
				cout << "BRANCH TAKEN" << endl;
				data_path -> pc = id_ex -> new_PC;
			}
		}
	}else {
		cout << "EXE STAGE - NOTHING TO EXECUTE FOR OPCODE: " << opcode << endl;

	}
	if(id_ex -> decoded_opcode !="syscall"){
		ex_mem -> rd = id_ex -> rd;

	}
	ex_mem -> rt = id_ex -> rt;
}


void memory_stage(DataPath *data_path, ex_mem_latch *ex_mem, mem_wb_latch *mem_wb){
		string opcode = ex_mem ->decoded_opcode;
		if( opcode == EMPTY_LATCH) {
			mem_wb -> decoded_opcode = ex_mem -> decoded_opcode;
			//No operation to be performed
			return;
		}
		else if (opcode == "nop") {
			mem_wb->decoded_opcode = "nop";
			cout << "MEM STAGE: NOP ENCOUNTERED" << endl;
			return;
		}
		else if (opcode == "lb") {
			mem_wb -> mdr = (data_path -> memory_read(ex_mem -> rt));
			cout << "MEM STAGE: LOADING BYTE " << mem_wb -> mdr << " FROM ADDR: " << ex_mem -> rt  << " WRITEBACK SET TO: " << ex_mem -> rd << endl;
			

		}
		// if we encounter an instruction that hasn't already finished and needs to go to write back stage
		// we forward all values for the writeback stage
		if(opcode != "lb" && opcode != "beqz" && opcode != "bge" && opcode != "bne"){
			cout << "MEM STAGE - MEM_WB BEING SET" << endl;
	    	mem_wb -> decoded_opcode = ex_mem -> decoded_opcode;
        	mem_wb -> alu_output = ex_mem -> alu_output;
       		mem_wb -> operand_b = ex_mem -> rt;
        	mem_wb -> rd = ex_mem -> rd;

		}
		else if (opcode == "lb"){
	    	mem_wb -> decoded_opcode = ex_mem -> decoded_opcode;
        	mem_wb -> rd = ex_mem -> rd;
		}
		else{
			mem_wb -> rd = SKIP_WRITEBACK; //Implicitly deny the wb_stage
			cout << "MEM STAGE - MEM_WB IS NOT BEING SET" << endl;
		}

}


void wb_stage (DataPath *data_path, mem_wb_latch *mem_wb, int* count){
	if (mem_wb -> rd){
		// load immediate, value already available in instruction
		string opcode = mem_wb -> decoded_opcode;
		if( opcode == EMPTY_LATCH) {
			//No operation to be performed
			return;
		}
		if (mem_wb -> rd == SKIP_WRITEBACK || opcode == "syscall") {
			*count = (*count) - 1;
			cout << "WB STAGE: SKIP WRITEBACK" << endl;
			return;
		}
		if (opcode == "nop") {
			//No operation to be performed but instruction has moved through the pipe
			*count = (*count) - 1;
			cout << "WB STAGE: NOP ENCOUNTERED" << endl;
			return;
		}
		string dest_reg = data_path -> decoder.registerDecode[mem_wb -> rd];

		if(opcode == "li" || opcode == "la"){
			data_path -> register_file.registers[dest_reg] = mem_wb -> operand_b;
			// fwd syscall value
		}
		if(opcode == "addi" || opcode == "subi" || opcode == "add"){
			data_path -> register_file.registers[dest_reg] = mem_wb -> alu_output;
		}
		if(opcode == "lb"){
			cout << "WRITING " << mem_wb -> mdr << " TO REGISTER" << endl;
			data_path -> register_file.registers[dest_reg] = mem_wb -> mdr;
		}
		// load from data, value shoudl be in mdr
		wb_debug(*data_path, mem_wb);
		*count = (*count) - 1; 

	} 
	else {
		cout << "WB STAGE: NOTHING TO WRITE BACK" << endl;
	}
	// instruction done
}


void if_debug(DataPath data_path, if_id_latch *if_id){
	printf("IF STAGE - INSTRUCTION FETCHED: %s \n", data_path.decoder.opcodeDecode[if_id->ir.operands[0]].c_str());
}

void id_debug(DataPath data_path, id_ex_latch *id_ex){
	string opcode = id_ex->decoded_opcode;
	//if(opcode == "add" || opcode == "addi" || opcode == "li" || opcode == "lb" || opcode == "subi"){
		printf("ID STAGE - INSTUCTION DECODED: %s, ALU OP: %d \n", id_ex -> decoded_opcode.c_str(), id_ex -> op);
	//}
}

void wb_debug(DataPath data_path, mem_wb_latch *mem_wb){
	string dest_reg = data_path.decoder.registerDecode[mem_wb -> rd];
	cout << "WB STAGE: wrote - " << data_path.register_file.registers[dest_reg] << " to register - " << dest_reg << endl;
}
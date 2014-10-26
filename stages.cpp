#include "stages.h"

void if_stage(DataPath *data_path, int *pc){
	Instruction current_instruction = data_path -> memory.at(*pc);
	// label?
	while(*pc < data_path -> memory.size() && data_path -> memory.at(*pc).type == "label"){
		*pc += 1;
		current_instruction = data_path -> memory.at(*pc);
	}
	// put instruction at program counter into instruction register
	data_path -> register_file.instruction_register = current_instruction;
	// increment pc
	*pc = *pc +=1;
	if_debug(*data_path);
}

// Read registers rs and rt in case we need them
// Compute the branch address in case the instruction is a branch
void id_stage(DataPath *data_path){
	string opcode = data_path -> decoder.opcodeDecode[data_path -> register_file.instruction_register.operands[0]];
	if( opcode == "addi" || opcode == "subi"){
		// set alu function code
		if(opcode == "addi"){
			data_path -> alu_function = 1;
		} else {
			data_path -> alu_function = 2;
		}
		// decode rs

		int rs = data_path -> register_file.instruction_register.operands[1];
		int rt = data_path -> register_file.instruction_register.operands[2];

		data_path -> register_file.instruction_register.operands[1] = data_path -> register_file.registers[data_path -> decoder.registerDecode[rs]];
		data_path -> register_file.instruction_register.operands[2] = data_path -> register_file.registers[data_path -> decoder.registerDecode[rt]];

		id_debug(*data_path);
		

	} else {
		data_path -> write_back = true;
	}

}
void execute_stage(DataPath *data_path){
	string opcode = data_path -> decoder.opcodeDecode[data_path -> register_file.instruction_register.operands[0]];
	if(opcode == "addi"){
		data_path -> alu_output = data_path -> alu(data_path -> register_file.instruction_register.operands[1], data_path -> register_file.instruction_register.operands[2]);		
		data_path -> write_back = true;
	}
}




void wb_stage (DataPath *data_path){
	if(data_path -> write_back){
		// load immediate, value already available in instruction
		string opcode = data_path -> decoder.opcodeDecode[data_path -> register_file.instruction_register.operands[0]];
		string dest_reg = data_path -> decoder.registerDecode[data_path -> register_file.instruction_register.operands[1]];
		if(opcode == "li"){
			data_path -> register_file.registers[dest_reg] = data_path -> register_file.instruction_register.operands[2];
			cout << "wrote: " << data_path -> register_file.registers[dest_reg] << " to register" << endl;
		}
		if(opcode == "addi" || opcode == "subi"){
			data_path -> register_file.registers[dest_reg] = data_path -> alu_output;
			cout << "wrote: " << data_path -> register_file.registers[dest_reg] << " to register" << endl;

		}
		// load from data, value shoudl be in mdr

		data_path -> write_back = false;
	}
}


void if_debug(DataPath data_path){
	printf("loaded instruction: %s \n", data_path.decoder.opcodeDecode[data_path.register_file.instruction_register.operands[0]].c_str());
}

void id_debug(DataPath data_path){
	if(data_path.register_file.instruction_register.type == "r-format")
	printf("decoded instruction: %s \n", data_path.decoder.opcodeDecode[data_path.register_file.instruction_register.operands[0]].c_str());
}
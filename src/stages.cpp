#include "../include/stages.h"

void id1_stage(DataPath *data_path, Scoreboard *scobo, int *cycle){
	Instruction fetched_instruction = data_path -> memory.at(data_path -> pc);
	Instruction *current_instruction;
	bool advance_pc = false; // set to true if we issue an instruction, otherwise stall on other funits

    // label? skip ot the next one.
	while(data_path -> pc < data_path -> memory.size() && data_path -> memory.at(data_path -> pc).type == "label"){
		data_path -> pc += 1;
		fetched_instruction = (data_path -> memory.at(data_path -> pc));
	}
	

	data_path -> fetch_buffer.push_back(fetched_instruction);


	// because the most recent instruction read in might not be the next one up for issuing.
	// while it relatively doesn't take a lot of time read in an instruction in c++ we wouldn't want to keep reading in
	// the instruction each time at the hardware level. so we store in a buffer.
	current_instruction = &(data_path -> fetch_buffer.at(data_path -> fetch_buffer.size() - 1));
	current_instruction -> status.ID1 = *cycle;

	// increment pc

	// decode the opcode to see if its functional unit is avail
	string opcode = data_path -> decoder.opcodeDecode[current_instruction -> operands[0]];
	


	if(opcode == "addi"|| opcode == "subi" || opcode == "add" || opcode == "li"){
		if(scobo -> fu_status[INTEGER].busy == true){
			// data_path -> fetch_buffer.pop_back();
			cout << "[ID1] :: INTEGER FU BUSY" << endl;
			return;
			// fu busy; stall aka do nothing.
		}
		
		else {
			// integer fu is free, set up scoreboard values. and issue instruction
			data_path -> integer_register_file.ir = (data_path -> fetch_buffer.size() - 1);
			scobo -> fu_status[INTEGER].busy = true;
			scobo -> fu_status[INTEGER].dirty = true;
			scobo -> fu_status[INTEGER].op = data_path -> decoder.opcodeDecode[current_instruction -> operands[0]];
			scobo -> fu_status[INTEGER].fi = data_path -> decoder.registerDecode[current_instruction -> operands[1]];
			scobo -> fu_status[INTEGER].fj = data_path -> decoder.registerDecode[current_instruction -> operands[2]];
			
			if (opcode == "add") {
				scobo -> fu_status[INTEGER].fk = data_path -> decoder.registerDecode[current_instruction -> operands[3]];
				scobo -> fu_status[INTEGER].rk = READY;
			} else {
				scobo -> fu_status[INTEGER].fk = current_instruction -> operands[3];
				scobo -> fu_status[INTEGER].rk = DONE;
			}
			// are the operand registers ready (DONE for rk cause it's immediate)

			scobo -> fu_status[INTEGER].rj = READY;	
			advance_pc = true;
		}

	}
	else if (opcode == "lid" || opcode == "fmul" || opcode == "fsub" || opcode == "sd" || opcode == "ld" || opcode == "fadd") {
		if (scobo -> fu_status[FLOAT].busy == true){
			cout << "[ID1] :: FLOAT FU BUSY" << endl;
			return;
		}
		else {
			// float fu is free
			data_path -> float_register_file.ir = (data_path -> fetch_buffer.size() - 1);
			scobo -> fu_status[FLOAT].busy = true;
			scobo -> fu_status[FLOAT].dirty = true;
			scobo -> fu_status[FLOAT].op = data_path -> decoder.opcodeDecode[current_instruction -> operands[0]];
			scobo -> fu_status[FLOAT].fi = data_path -> decoder.registerDecode[current_instruction -> operands[1]];
			scobo -> fu_status[FLOAT].fj = data_path -> decoder.registerDecode[current_instruction -> operands[2]];
				cout << scobo->fu_status[FLOAT].fj << endl;
			if (opcode == "lid") {
				scobo -> fu_status[FLOAT].fk = current_instruction -> operands[3];
				scobo -> fu_status[FLOAT].rk = DONE;
			}
			else if (opcode == "ld" || opcode == "sd"){
				scobo -> fu_status[FLOAT].fk = current_instruction -> operands[3];
				scobo -> fu_status[FLOAT].rk = DONE;
			}
			else {
				scobo -> fu_status[FLOAT].fk = data_path -> decoder.registerDecode[current_instruction -> operands[3]];
				scobo -> fu_status[FLOAT].rk = READY;
			}

			scobo -> fu_status[FLOAT].rj = READY;
			advance_pc = true;
		}
	}
	else if (opcode == "nop")
	{
		advance_pc = data_path -> timeout_count > 2;
	}

	cout << "[ID1] :: HERE" << endl;
	 
	if(advance_pc){
		data_path -> pc = (data_path -> pc) + 1;
		data_path -> timeout_count = 0;
	} else {
		data_path -> timeout_count += 1; // for debugging, time out if we stall for too long.
		cout << "[ID1] STALLING ON OPCODE: " << opcode << endl;
		if(data_path -> timeout_count > TIMEOUT_LIMIT){
			data_path -> user_mode = false;
			cout << endl << "[ID1] :: TIME OUT: STALLED FOR MORE THAN " << TIMEOUT_LIMIT << " CYCLES" << endl;
		}
	}

}


// if pipelined is true. we pipeline the decode ex stage.
void id2_stage(DataPath *data_path, Scoreboard *scobo, int* cycle){

	// ID STAGE 2 FOR INTEGER FU
	// Don't do anything if both are already DONE (only possible if they've gone through once)
	if(scobo -> fu_status[INTEGER].dirty != true && (scobo->fu_status[INTEGER].rj == READY || scobo->fu_status[INTEGER].rk == READY)){
		// this is vastly simplified, we don't need to have all this conditional logic
		// depending on the instruction anymore. we just check if there registers that arent ready.
		// if they aren't ready, we can check if the fu they're waiting on is complete. if Qj / Qk
		// == NONE then we'll just assume its okay to read, because it isn't waiting on a

		cout << "[ID2] :INTEGER: fj = " << scobo->fu_status[INTEGER].fj << " fk = " << scobo->fu_status[INTEGER].fk << endl;

		if(scobo -> fu_status[INTEGER].rj == READY){
			// place decoded operands into temporary registers, should be fine since they are ready to be read and will execute next up
	     	data_path -> integer_register_file.registers["$t0"] = data_path -> register_file.registers[scobo -> fu_status[INTEGER].fj];
	     	// update scoreboard
			scobo -> fu_status[INTEGER].rj = DONE;
			// other value should be immediate so we should be good to go to exec stage.
		}
		if(scobo -> fu_status[INTEGER].rk == READY){
	     	data_path -> integer_register_file.registers["$t1"] = data_path -> register_file.registers[scobo -> fu_status[INTEGER].fk];
	     	// update scoreboard
			scobo -> fu_status[INTEGER].rk = DONE;
			// other value should be immediate so we should be good to go to exec stage.
		}	
		cout << "[ID2] :: Loaded into Temp registers" << endl;
		data_path ->  fetch_buffer.at(data_path -> integer_register_file.ir).status.ID2 = *cycle;
		scobo -> fu_status[INTEGER].dirty = true;
	// if(scobo -> fu_status[INTEGER].rj == NOTREADY || scobo -> fu_status[INTEGER].rk == NOTREADY ){
	// 	id_ex -> nop = true; //one operand isn't ready we need to wait
	// }

	}

	if(scobo -> fu_status[FLOAT].dirty != true && (scobo->fu_status[FLOAT].rj == READY || scobo->fu_status[FLOAT].rk == READY)){
		cout << "[ID2] :FLOAT: fj = " << scobo->fu_status[FLOAT].fj << " fk = " << scobo->fu_status[FLOAT].fk << endl;
		
		if(scobo -> fu_status[FLOAT].rj == READY){
			data_path -> float_register_file.registers["$t0"] = data_path -> register_file.registers[scobo -> fu_status[FLOAT].fj];
			scobo -> fu_status[FLOAT].rj = DONE;
		}

		if(scobo -> fu_status[FLOAT].rk == READY){
	     	data_path -> float_register_file.registers["$t1"] = data_path -> register_file.registers[scobo -> fu_status[FLOAT].fk];
			scobo -> fu_status[FLOAT].rk = DONE;
		}	

		cout << "[ID2] :: Loaded into Temp registers" << endl;
		data_path ->  fetch_buffer.at(data_path -> float_register_file.ir).status.ID2 = *cycle;
		scobo -> fu_status[FLOAT].dirty = true;
	}	
}

void execute_stage(DataPath *data_path, Scoreboard *scobo, int* cycle ){

	// do for each functional unit, if both are done, it implicitly means we're in the execute stage
		if(scobo -> fu_status[INTEGER].dirty != true && scobo -> fu_status[INTEGER].result_ready != true
			&& scobo -> fu_status[INTEGER].rj == DONE && scobo -> fu_status[INTEGER].rk == DONE){
			int rj, rk;
			opcode op =	data_path -> decoder.opcodeEnumDecode[scobo -> fu_status[INTEGER].op];
	     	rj = data_path -> integer_register_file.registers["$t0"];

	     	if (op == ADD)
	     		rk = data_path -> integer_register_file.registers["$t1"];
	     	else if (op == LI)
	     		rk = data_path -> fetch_buffer.at(data_path -> integer_register_file.ir).operands[2];
	     	else
	     		rk = data_path -> fetch_buffer.at(data_path -> integer_register_file.ir).operands[3];


	     	cout << "[EXE] :: rj = " << rj << " rk = " << rk << endl;
			switch(op){
				case ADDI:
					data_path -> integer_register_file.registers["$t2"] = rj + rk;
					break;
				case ADD:
					data_path -> integer_register_file.registers["$t2"] = rj + rk;
					break;
				case SUBI:
					data_path -> integer_register_file.registers["$t2"] = rj - rk;
					break;
				case LI:
					data_path -> integer_register_file.registers["$t2"] = rk;
					break;
				default:
					// "this can be removed when we put all enum opcode cases"
					break;
			}

			cout << "[EXE] :INTEGER: EXE Result Loaded into Temp register" << endl;
			data_path -> fetch_buffer.at(data_path -> integer_register_file.ir).status.EX = *cycle;
			scobo -> fu_status[INTEGER].dirty = true;
			scobo -> fu_status[INTEGER].result_ready = true;
		}

		if(scobo -> fu_status[FLOAT].dirty != true && scobo -> fu_status[FLOAT].result_ready != true
			&& scobo -> fu_status[FLOAT].rj == DONE && scobo -> fu_status[FLOAT].rk == DONE){
			float rj, rk;
			opcode op =	data_path -> decoder.opcodeEnumDecode[scobo -> fu_status[FLOAT].op];
	     	float ri = data_path -> register_file.registers[scobo -> fu_status[FLOAT].fi];

	    	rj = data_path -> float_register_file.registers["$t0"];
	     	if (op == FMUL || op == FSUB || op == FADD)
	     		rk = data_path -> float_register_file.registers["$t1"];
	     	else if (op == LID)
	     		rk = data_path -> fetch_buffer.at(data_path -> float_register_file.ir).float_operands[0];
	     	else
	     		rk = data_path -> fetch_buffer.at(data_path -> float_register_file.ir).operands[3];


	     	cout << "[EXE] :FLOAT: rj = " << rj << " rk = " << rk << endl;
			switch(op){
				case LID:
					data_path -> float_register_file.registers["$t2"] = rk;
					break;
				case FMUL:
					data_path -> float_register_file.registers["$t2"] = rk * rj;
					break;
				case FSUB:
					data_path -> float_register_file.registers["$t2"] = rj - rk;
					break;
				case FADD:
					data_path -> float_register_file.registers["$t2"] = rk + rj;
					break;
				case SD:
					data_path -> float_register_file.registers["$t2"] = ri;
					break;
				case LD:
					data_path -> float_register_file.registers["$t2"] = rj;
					break;
				default:
					// "this can be removed when we put all enum opcode cases"
					break;
			}

			cout << "[EXE] :: EXE Result Loaded into Temp register" << endl;
			data_path -> fetch_buffer.at(data_path -> float_register_file.ir).status.EX = *cycle;
			scobo -> fu_status[FLOAT].dirty = true;
			scobo -> fu_status[FLOAT].result_ready = true;
		}
}

void writeback_stage(DataPath *data_path, Scoreboard *scobo, int* cycle){

	if(scobo -> fu_status[INTEGER].dirty != true && scobo -> fu_status[INTEGER].result_ready){
		
		string dest_reg = scobo -> fu_status[INTEGER].fi;


		data_path -> register_file.registers[dest_reg] = data_path -> integer_register_file.registers["$t2"];
		cout << "[WB] :: WRITING " << data_path -> register_file.registers[dest_reg] << " TO REGISTER " << dest_reg << endl;


		//Write back is complete, free the Integer fu
		scobo -> fu_status[INTEGER].busy = false;
		scobo -> fu_status[INTEGER].dirty = false;
		scobo -> fu_status[INTEGER].result_ready = false;
		scobo -> fu_status[INTEGER].fi = "";
		scobo -> fu_status[INTEGER].fj = "";
		scobo -> fu_status[INTEGER].fk = "";
		scobo -> fu_status[INTEGER].rj = NOTREADY;
		scobo -> fu_status[INTEGER].rk = NOTREADY;

		data_path -> fetch_buffer.at(data_path -> integer_register_file.ir).status.WB = *cycle;
	}

	if(scobo -> fu_status[FLOAT].dirty != true && scobo -> fu_status[FLOAT].result_ready){
		opcode op =	data_path -> decoder.opcodeEnumDecode[scobo -> fu_status[FLOAT].op];	
		string dest_reg = scobo -> fu_status[FLOAT].fi;
		
		if (op == SD)
			dest_reg = scobo -> fu_status[FLOAT].fj;

		data_path -> register_file.registers[dest_reg] = data_path -> float_register_file.registers["$t2"];
		cout << "[WB] :: WRITING " << data_path -> register_file.registers[dest_reg] << " TO REGISTER " << dest_reg << endl;


		//Write back is complete, free the FLOAT fu
		scobo -> fu_status[FLOAT].busy = false;
		scobo -> fu_status[FLOAT].dirty = false;
		scobo -> fu_status[FLOAT].result_ready = false;
		scobo -> fu_status[FLOAT].fi = "";
		scobo -> fu_status[FLOAT].fj = "";
		scobo -> fu_status[FLOAT].fk = "";
		scobo -> fu_status[FLOAT].rj = NOTREADY;
		scobo -> fu_status[FLOAT].rk = NOTREADY;

		data_path -> fetch_buffer.at(data_path -> float_register_file.ir).status.WB = *cycle;
	}
}


void if_debug(DataPath data_path, if_id_latch *if_id){
	// printf("IF STAGE - INSTRUCTION FETCHED: %s \n", data_path.decoder.opcodeDecode[if_id->ir.operands[0]].c_str());
}

void id_debug(DataPath data_path, id_ex_latch *id_ex){
	string opcode = id_ex->decoded_opcode;
	//if(opcode == "add" || opcode == "addi" || opcode == "li" || opcode == "lb" || opcode == "subi"){
		// printf("ID STAGE - INSTUCTION DECODED: %s, ALU OP: %d \n", id_ex -> decoded_opcode.c_str(), id_ex -> op);
	//}
}

void wb_debug(DataPath data_path, mem_wb_latch *mem_wb){
	// string dest_reg = data_path.decoder.registerDecode[mem_wb -> rd];
	// cout << "WB STAGE: wrote - " << data_path.register_file.registers[dest_reg] << " to register - " << dest_reg << endl;
}
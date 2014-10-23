#include "loader.h"
#include <sstream>
#include <iostream>
#include <limits>
#include <string>
#include <cstdlib>
#include "instruction.h"

Loader::Loader(string name){
	file_name = name;
}

// get the length of the text segment 

int Loader::text_segment_length(){
	int i = 0;
    string arr[4];
	string line;
	source_file.open(file_name);


	if (source_file.is_open())
		{
			bool in_text_segment = false;
			while ( getline (source_file, line) )
				{
		    		stringstream ssin(line);
					while (i < 4 && ssin.good()){
				        ssin >> arr[i];
			    	}
			    	if(arr[0] == ".text"){
			    		in_text_segment = true;
			    	}
			    	else if (arr[0] == ".data"){
			    		in_text_segment = false;
			    	}
			    	if(in_text_segment){
			    		i++;
			    	}
				}
		}
	source_file.close();
	return i;
}



// open file, read lines, parse into instruction, store in memory unit
int Loader::parse_assembly(vector<Instruction>* memory, Decoder decoder){
	string line;
	source_file.open(file_name);
	if (source_file.is_open()){
		int j = 0;
		while ( getline (source_file, line) ){
		    
			vector<string> operands; //dynamic number of operands
		    int i = 0;
		    stringstream ssin(line);
		    while (ssin.good()){
		    	string temp;
		        ssin >> temp;
		        if(strncmp(temp.c_str(), "#", 1) != 0){

			        if (i == 1){
			    	  temp.pop_back(); // remove comma separator
			        }
			        else if(i == 2 && (operands[0] == "addi" || operands[0] == "subi")){
			    	  temp.pop_back(); // remove second comma separator
			        }
			        operands.push_back(temp);
			        ++i;
			    }
			    else {
			    	break; // means we found a comment 
			    }
		    }
		    if(operands.size() > 0 && (operands[0] == "addi" || operands[0] == "subi")){
			    translate_rformat_to_binary(memory, j, operands, i, decoder);
			    j++;
		    }
		    else if(operands.size() > 0 && (operands[0] == "li" || operands[0] == "lb")){
		    	translate_iformat_to_binary(memory, j, operands, i, decoder);
			    j++;	
		    }
		    else if(operands.size() > 0 && (operands[0] == "b" || operands[0] == "bge"|| operands[0] == "bne"|| operands[0] == "beqz" || operands[0] == "la" || operands[0] == "syscall")){
		    	// translate_iformat_to_binary(memory, j, operands, i, decoder);
			    // j++;	
		    }
		    else if(operands.size() > 0 && operands[0].back() == ':'){
		    	memory -> at(j).type = operands[0];
				loader_debug(memory, j);		    	
		    }
		}
		source_file.close();
	}

	else cout << "Unable to open file"; 

	return 0;
}
//ADDI, B, BEQZ, BGE, BNE, LA, LB, LI, SUBI, and SYSCALL. xxx


// writes the binary representation of an instruction to our memory unit
void Loader::translate_rformat_to_binary(vector<Instruction>* memory, int next_memory_slot_index, vector<string> tokens, int length, Decoder decoder){
	string opcode = tokens[0];
	
	//ADDI, SUBI instructions
	if(length == 4){
		if(opcode == "addi"){
			memory -> at(next_memory_slot_index).operands.push_back(00000);
		} else {
			memory -> at(next_memory_slot_index).operands.push_back(00001);
		}
		// rs
		memory -> at(next_memory_slot_index).operands.push_back(decoder.registerEncode[tokens[1]]);
		// rt
		memory -> at(next_memory_slot_index).operands.push_back(decoder.registerEncode[tokens[2]]);
		// rd
		memory -> at(next_memory_slot_index).operands.push_back(atoi(tokens[3].c_str()));
		memory -> at(next_memory_slot_index).type = "r-format";

		loader_debug(memory, next_memory_slot_index);
	} else {
		printf("\nPlease check your r-format instructions. it appears an operand is missing for one(or more) instructions.\n");
	}
}

void Loader::translate_iformat_to_binary(vector<Instruction>* memory, int next_memory_slot_index, vector<string> tokens, int length, Decoder decoder){
	string opcode = tokens[0];
	
	//ADDI, SUBI instructions
	if(length == 3){
		if(tokens[0] == "li"){
			memory -> at(next_memory_slot_index).operands.push_back(00010);
			memory -> at(next_memory_slot_index).operands.push_back(decoder.registerEncode[tokens[1]]);
			memory -> at(next_memory_slot_index).operands.push_back(atoi(tokens[2].c_str()));
		}
		else if(tokens[0] == "lb"){
			memory -> at(next_memory_slot_index).operands.push_back(00011);
			memory -> at(next_memory_slot_index).operands.push_back(decoder.registerEncode[tokens[1]]);
			//remove parentheses from string
			tokens[2].erase(0, 1); 
			tokens[2].pop_back();
			memory -> at(next_memory_slot_index).operands.push_back(decoder.registerEncode[tokens[1]]);			
		}

		memory -> at(next_memory_slot_index).type = "i-format";
		loader_debug(memory, next_memory_slot_index);
	} else {
		printf("\nPlease check your i-format instructions. it appears an operand is missing for one(or more) instructions.\n");
	}
}






void Loader::loader_debug(vector<Instruction>* memory, int index){
	printf("Instruction: %s", memory -> at(index).type.c_str());
	// the i'th instruction's operands
	if(memory -> at(index).operands.size() > 0){
		printf("opcode: %d ", memory -> at(index).operands[0]);
		int last = memory -> at(index).operands.size() - 1;
	    for(int i = 0; i < last; ++i) {
	    	printf("operand %d: %d", i + 1, memory -> at(index).operands[i + 1]);

	    }
	}
    printf("\n");

}
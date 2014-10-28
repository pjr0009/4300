#include "loader.h"
#include <sstream>
#include <iostream>
#include <limits>
#include <string>
#include <cstdlib>

Loader::Loader(string name){
	file_name = name;
}

// get the length of the text segment 

int Loader::text_segment_length(){
	int i = 0;
	string line;
	source_file.open(file_name.c_str());


	if (source_file.is_open())
		{
			bool in_text_segment = false;
			while ( getline (source_file, line) )
				{
		    		string token;
		    		stringstream ssin(line);
					if(ssin.good()){
						ssin >> token;
					}
			    	if(token == ".text"){
			    		in_text_segment = true;
			    	}
			    	else if (token == ".data"){
			    		in_text_segment = false;
			    	}
			    	if(in_text_segment){
			    		if (token[0] != '#' && token[0] != '\0' && token[0] != '.'){
				    		i++;
			    		}
			    	}
				}
		}
	source_file.close();
	return i;
}



// open file, read lines, parse into instruction, store in memory unit
int Loader::parse_assembly(DataPath* data_path){
	string line;
	source_file.open(file_name);
	if (source_file.is_open()){
		int j = 0;
		while ( getline (source_file, line) ){
		    
			vector<string> operands; //dynamic number of operands
		    int i = 0;
			stringstream ssin(line.erase(line.find_last_not_of(" \n\r\t") +1));
		    while (ssin.good()){
		    	string temp;
		        ssin >> temp;
		        if(strncmp(temp.c_str(), "#", 1) != 0){

			        if (i == 1 && (operands[0] != "b")){
			    	  temp.pop_back(); // remove comma separator
			        }
			        else if(i == 2 && (operands[0] == "addi" || operands[0] == "subi" || operands[0] == "add" || operands[0] == "bge")){
			    	  temp.pop_back(); // remove second comma separator
			        }
			        operands.push_back(temp);
			        ++i;
			    }
			    else {
			    	break; // means we found a comment 
			    }
		    }


		    // translate instruction based on type
		    if(operands.size() > 0 && (operands[0] == "addi" || operands[0] == "subi" || operands[0] == "add")){
			    translate_rformat_to_binary(data_path, j, operands, i);
			    j++;
		    }
		    else if(operands.size() > 0 && (operands[0] == "li" || operands[0] == "lb" || operands[0] == "la" || operands[0] == "bge" || operands[0] == "beqz")){
		    	translate_iformat_to_binary(data_path, j, operands, i);
			    j++;	
		    }
		    else if(operands.size() > 0 && (operands[0] == "b" || operands[0] == "bne")){
		    	translate_jformat_to_binary(data_path, j, operands, i);
			    j++;	
		    }

		    //syscall
		    else if(operands.size() > 0 && operands[0] == "syscall"){
		    	data_path -> memory.at(j).type = "syscall";
				data_path -> memory.at(j).operands.push_back(01000);
		    	loader_debug(*data_path, j);
				j++;

		    }

		    else if(operands.size() > 0 && operands[0] == "nop"){
		    	data_path -> memory.at(j).type = "nop";
		    	data_path -> memory.at(j).operands.push_back(11111);
		    	loader_debug(*data_path, j);
		    	j++;
		    }

		    //label
		    else if(operands.size() > 0 && operands[0].back() == ':'){
		    	operands[0].pop_back();
		    	data_path -> memory.at(j).type = "label";
		    	data_path -> memory.at(j).label = operands[0];

		    	data_path -> memory.at(j).operands.push_back(j); // offset
				loader_debug(*data_path, j);		
				j++;    	
		    } 

		}

		source_file.close();
	}

	else cout << "Unable to open file"; 

	return 0;
}
//ADDI, B, BEQZ, BGE, BNE, LA, LB, LI, SUBI, and SYSCALL. xxx


// writes the binary representation of an instruction to our memory unit
void Loader::translate_rformat_to_binary(DataPath *data_path, int next_memory_slot_index, vector<string> tokens, int length){
	string opcode = tokens[0];
	
	//ADDI, SUBI instructions
	if(length == 4){
		if(opcode == "addi"){
			data_path -> memory.at(next_memory_slot_index).operands.push_back(00000);
		} else if (opcode == "subi") {
			data_path -> memory.at(next_memory_slot_index).operands.push_back(00001);
		} else if (opcode == "add"){
			data_path -> memory.at(next_memory_slot_index).operands.push_back(00110);

		}
		// rd
		data_path -> memory.at(next_memory_slot_index).operands.push_back(data_path -> decoder.registerEncode[tokens[1]]);
		// rs
		data_path -> memory.at(next_memory_slot_index).operands.push_back(data_path -> decoder.registerEncode[tokens[2]]);
		
		// rt
		if(opcode == "add"){
			data_path -> memory.at(next_memory_slot_index).operands.push_back(data_path -> decoder.registerEncode[tokens[3]]);
			data_path -> memory.at(next_memory_slot_index).type = "r-format";
		
		}

		//otherwise rt is an immediate value
		else {
			data_path -> memory.at(next_memory_slot_index).operands.push_back(atoi(tokens[3].c_str()));
			data_path -> memory.at(next_memory_slot_index).type = "i-format";

		}

		loader_debug(*data_path, next_memory_slot_index);
	} else {
		printf("\nPlease check your r-format instructions. it appears an operand is missing for one(or more) instructions.\n");
	}
}

void Loader::translate_jformat_to_binary(DataPath* data_path, int next_memory_slot_index, vector<string> tokens, int length){
	string opcode = tokens[0];
	
	if(opcode == "b"){
		data_path -> memory.at(next_memory_slot_index).operands.push_back(00100);
		data_path -> memory.at(next_memory_slot_index).label = tokens[1];
		data_path -> memory.at(next_memory_slot_index).type = "j-format";
		loader_debug(*data_path, next_memory_slot_index);
	} else {
		// printf("\nPlease check your r-format instructions. it appears an operand is missing for one(or more) instructions.\n");
	}
}

void Loader::translate_iformat_to_binary(DataPath* data_path, int next_memory_slot_index, vector<string> tokens, int length){
	string opcode = tokens[0];
	
	//ADDI, SUBI instructions
	if(tokens[0] == "li"){
		data_path -> memory.at(next_memory_slot_index).operands.push_back(00010);
		data_path -> memory.at(next_memory_slot_index).operands.push_back(data_path -> decoder.registerEncode[tokens[1]]);
		data_path -> memory.at(next_memory_slot_index).operands.push_back(atoi(tokens[2].c_str()));
	}
	else if(tokens[0] == "lb"){
		data_path -> memory.at(next_memory_slot_index).operands.push_back(00011);
		data_path -> memory.at(next_memory_slot_index).operands.push_back(data_path -> decoder.registerEncode[tokens[1]]);
		//remove parentheses from string
		tokens[2].erase(0, 1); 
		tokens[2].pop_back();
		data_path -> memory.at(next_memory_slot_index).operands.push_back(data_path -> decoder.registerEncode[tokens[1]]);			
	}
	else if(tokens[0] == "la"){
		// // iterate through memory and replace the offset of label in la
		// int last = memory -> size() - 1;
	 //    for(int i = 1; i < last; ++i) {
	 //    	if()
	 //    }
	}
	else if(tokens[0] == "beqz"){
		data_path -> memory.at(next_memory_slot_index).operands.push_back(00111);
		data_path -> memory.at(next_memory_slot_index).operands.push_back(data_path -> decoder.registerEncode[tokens[1]]);
		data_path -> memory.at(next_memory_slot_index).label = tokens[2];

	}

	else if (opcode == "bge") {
		data_path -> memory.at(next_memory_slot_index).operands.push_back(00101);
		// rs
		data_path -> memory.at(next_memory_slot_index).operands.push_back(data_path -> decoder.registerEncode[tokens[1]]);
		// rt
		data_path -> memory.at(next_memory_slot_index).operands.push_back(data_path -> decoder.registerEncode[tokens[2]]);
		// rd
		data_path -> memory.at(next_memory_slot_index).label = tokens[3];

		data_path -> memory.at(next_memory_slot_index).type = "i-format";

		
	}
	data_path -> memory.at(next_memory_slot_index).type = "i-format";
	loader_debug(*data_path, next_memory_slot_index);
}

void Loader::loader_debug(DataPath data_path, int index){
	string type = data_path.memory.at(index).type;
	vector<int> operands = data_path.memory.at(index).operands;
	printf("\n Instruction type: %s, ", type.c_str());
	// the i'th instruction's operands
	if(operands.size() > 0){
	    if(type == "label"){
	    	printf("name: %s, offset: %d ", data_path.memory.at(index).label.c_str(), operands[0]);
	    } else {
	    	if(data_path.memory.at(index).label.size() > 0){
	    		printf("label: %s, ", data_path.memory.at(index).label.c_str());
	    	}
			printf("opcode: %s ", data_path.decoder.opcodeDecode[operands[0]].c_str());
			int last = operands.size() - 1;
		    for(int i = 0; i < last; ++i) {
		    	printf("operand %d: %d ", i + 1, operands[i + 1]);
		    }
	    }

	}
    printf("\n");

}
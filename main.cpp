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

    int PC = 0;

    while(PC < data_path.memory.size()){
    	// instruction fetch
    	if_stage(&data_path, &PC);
    	

    	// instruction decode
    	id_stage(&data_path);

    	// execute 
    	execute_stage(&data_path);

    	//write back
    	wb_stage(&data_path);
    	// execute
    	// memory
    	// write back

    	// execute();
    }


	return 0;
}
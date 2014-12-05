#include "../include/scoreboard.h"
#include <iostream>
#include <iomanip>

Scoreboard::Scoreboard(int instruction_count, DataPath data_path) {

	// create functional unit status
	for (int i=0; i < NUM_OF_FU; i++){
		struct functional_unit_status_entry entry;
		entry.busy = false;
		entry.fi = "";
		entry.fj = "";
		entry.fk = "";
		entry.rj = NOTREADY;
		entry.rk = NOTREADY;
		entry.qj = NONE;
		entry.qk = NONE;
		switch(i){
			case INTEGER:
				entry.name = "Integer";
				break;
			case FLOAT:
				entry.name = "Float";
				break;
		}
		fu_status.push_back(entry);
	}
	// create register status

}

template<typename T> void printElement(T t, const int& width)
{
   	const char separator    = ' ';
    cout << left << setw(width) << setfill(separator) << t;
}

void Scoreboard::debug(DataPath *data_path){
	cout << endl;
	printElement("*************SCOREBOARD***********", 40);
	cout << endl;
	printElement("\nINSTRUCTION STATUS", 40);
	cout << endl;
	cout << endl;
	printElement("ISSUE", 10);
	printElement("DECODE", 10);
	printElement("EXEC", 10);
	printElement("WRITE", 10);
	cout << endl;

		

			instruction_status_entry entry = data_path -> integer_register_file.ir.status;
			if(entry.ID1 != 0){
				printElement(entry.ID1, 10);
			}

			if(entry.ID2 != 0){
				printElement(entry.ID2, 10);
			}			

			if(entry.EX != 0){
				printElement(entry.EX, 10);
			}			
		    if(entry.WB != 0){
				printElement(entry.WB, 10);
			}

			cout << endl;
		    entry = data_path -> float_register_file.ir.status;
			if(entry.ID1 != 0){
				printElement(entry.ID1, 10);
			}

			if(entry.ID2 != 0){
				printElement(entry.ID1, 10);
			}			

			if(entry.EX != 0){
				printElement(entry.ID1, 10);
			}			
		    if(entry.WB != 0){
				printElement(entry.ID1, 10);
			}

			cout << endl;

	// functional unit status
	cout << endl;
	printElement("\nFUNCTIONAL UNIT STATUS\n", 40);
	cout << endl;

	printElement("UNIT", 10);
	printElement("BUSY", 10);
	printElement("OP", 10);
	printElement("FI", 10);
	printElement("FJ", 10);
	printElement("FK", 10);
	printElement("QJ", 10);
	printElement("QK", 10);
	printElement("RJ", 10);
	printElement("RK", 10);
	cout << endl;

	for(int i = 0; i < NUM_OF_FU; i++){
		functional_unit_status_entry fu_entry = fu_status[i];
		printElement(fu_entry.name, 10);
		printElement(fu_entry.busy, 10);
		printElement(fu_entry.op, 10);
		printElement(fu_entry.fi, 10);
		printElement(fu_entry.fj, 10);
		printElement(fu_entry.fk, 10);
		if(fu_entry.qj == NONE){
			printElement("", 10);

		} else{
			printElement(fu_entry.qj, 10);

		}
	    if(fu_entry.qk == NONE){
			printElement("", 10);

		} else{
			printElement(fu_entry.qk, 10);

		}
		
		switch (fu_entry.rj){
			case READY: 
				printElement("READY", 10);
				break;
			case NOTREADY: 
				printElement("NOTREADY", 10);
				break;
			case DONE: 
				printElement("DONE", 10);
				break;
		}

		switch (fu_entry.rk){
			case READY: 
				printElement("READY", 10);
				break;
			case NOTREADY: 
				printElement("NOTREADY", 10);
				break;
			case DONE: 
				printElement("DONE", 10);
				break;
		}

		cout << endl;


	}

}
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
		entry.rj = false;
		entry.rk = false;
		entry.qj = NONE;
		entry.qk = NONE;
		fu_status.push_back(entry);
	}

	// create instruction status board
	for (int i=0; i <= instruction_count; i++){
		struct instruction_status_entry entry;
		entry.ID1 = 0;
		entry.ID2 = 0;
		entry.EX  = 0;
		entry.WB  = 0;
		instruction_status.push_back(entry);
	}
	// create register status

}

template<typename T> void printElement(T t, const int& width)
{
   	const char separator    = ' ';
    cout << left << setw(width) << setfill(separator) << t;
}

void Scoreboard::debug(){
	cout << endl;
	printElement("*************SCOREBOARD***********", 40);
	cout << endl;
	printElement("INSTRUCTION STATUS", 40);
	cout << endl;
	cout << endl;
	printElement("ID1", 10);
	printElement("ID2", 10);
	printElement("EX", 10);
	printElement("WB", 10);

		

	for(int i = 0; i < instruction_status.size(); i++){
			if(instruction_status[i].ID1 != 0){
				printElement(instruction_status[i].ID1, 10);
			}

			if(instruction_status[i].ID2 != 0){
				printElement(instruction_status[i].ID1, 10);
			}			

			if(instruction_status[i].EX != 0){
				printElement(instruction_status[i].ID1, 10);
			}			
		    if(instruction_status[i].WB != 0){
				printElement(instruction_status[i].ID1, 10);
			}

			cout << endl;
	}

	// functional unit status
	cout << endl;
	printElement("FUNCTIONAL UNIT STATUS", 40);
	cout << endl;
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
		printElement(fu_entry.rj, 10);
		printElement(fu_entry.rk, 10);
		cout << endl;


	}

}
#ifndef _SCOREBOARD_H
#define _SCOREBOARD_H

#include <vector>
#include "decoder.h"
#include "data_path.h"
#include "instruction.h"

#define NUM_OF_FU 2
typedef int clock_cycle;

enum functional_unit {INTEGER, FLOAT, NONE};


struct functional_unit_status_entry {
	bool busy; //fu is busy
	string op; //if fu is processing instruction, what kind?
	
	string fi;
	string fj; // fu register names that are being used
	string fk;

	functional_unit qj;
	functional_unit qk; //Qj, Qk: Name of functional unit producing regs Fj, Fk
	
	bool rj;
	bool rk;  //Flags indicating when Fj and Fk are ready 
};

struct instruction_status_entry {
	Instruction instruction;
	clock_cycle ID1; 
	clock_cycle ID2;
	clock_cycle EX;
	clock_cycle WB;
};

class Scoreboard {
public:
	/* index of where the instruction lives in the code segment.*/
	int instruction_id; 
	Scoreboard(int instruction_count, DataPath data_path);
	void debug();
	/* instruction status - one entry per instruction    */
	/* tells the clock cycle at which each stage happens */
	vector<instruction_status_entry> instruction_status;
	vector<functional_unit_status_entry> fu_status; 


	/* functional unit status - one entry per funcitonal unit */
	/* tracks the status of the each functional unit */


};

#endif
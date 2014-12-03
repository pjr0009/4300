#include <vector>
#include "decoder.h"

typedef short clock_cycle;

enum functional_unit {NONE, INTEGER, FLOAT};


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
	clock_cycle ID; 
	clock_cycle ID2;
	clock_cycle EX;
	clock_cycle WB;
};

class Scoreboard {
public:
	/* index of where the instruction lives in the code segment.*/
	int instruction_id; 

	/* instruction status - one entry per instruction    */
	/* tells the clock cycle at which each stage happens */
	vector<instruction_status_entry> instruction_status; 

	/* functional unit status - one entry per funcitonal unit */
	/* tracks the status of the each functional unit */


};
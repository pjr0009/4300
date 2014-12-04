#include "../include/scoreboard.h"


Scoreboard::Scoreboard(int instruction_count) {
	int j = 1;

	// create functional unit status
	for (int i=0; i <= j; i++){
		struct functional_unit_status_entry entry;
		fu_status.push_back(entry);
	}

	// create instruction status board
	for (int i=0; i <= instruction_count; i++){
		struct instruction_status_entry entry;
		instruction_status.push_back(entry);
	}
	// create register status

}
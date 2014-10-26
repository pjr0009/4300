#ifndef _INSTRUCTION_H
#define _INSTRUCTION_H
#include <vector>
#include <string>
using namespace std;

class Instruction {

	public:
	  string type;
	  string label; //optional, in realworld would be an binary encoded value
	  vector<int> operands;


};

#endif
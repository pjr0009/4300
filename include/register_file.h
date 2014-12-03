#include <map>
#include <string>
#include "instruction.h"
using namespace std;
class RegisterFile {
	public:
		RegisterFile();
		Instruction instruction_register;
		map<string, int> registers;

};

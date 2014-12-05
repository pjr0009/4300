#include <map>
#include <string>
#include "instruction.h"
using namespace std;
class RegisterFile {
	public:
		RegisterFile();
		Instruction ir;
		map<string, int> registers;

};

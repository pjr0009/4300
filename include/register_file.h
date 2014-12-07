#include <map>
#include <string>
#include "instruction.h"
using namespace std;
class RegisterFile {
	public:
		RegisterFile();
		int ir;
		map<string, float> registers;

};

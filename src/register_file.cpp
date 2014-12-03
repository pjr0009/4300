#include "../include/register_file.h"

RegisterFile::RegisterFile(){
    registers["zero"]  = 0;
	registers["$at"]   = 0;
	registers["$v0"]   = 0;
	registers["$v1"]   = 0;
	registers["$a0"]   = 0;
	registers["$a1"]   = 0;
	registers["$a2"]   = 0;
	registers["$a3"]   = 0;
	registers["$t0"]   = 0;
	registers["$t1"]   = 0;
	registers["$t2"]   = 0;
	registers["$t3"]   = 0;
	registers["$t3"]   = 0;
	registers["$t4"]   = 0;
	registers["$t5"]   = 0;
	registers["$t6"]   = 0;
	registers["$t7"]   = 0;
}
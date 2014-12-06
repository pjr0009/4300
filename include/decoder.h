#ifndef _DECODER_H
#define _DECODER_H

#include <string.h>
#include <iostream>
#include <map>
#include <utility>

using namespace std;
enum opcode {ADDI, SUBI, LI, LB, B, BGE, ADD, BEQZ, SYSCALL, BNE, LA, NOP};
class Decoder {
	public:
		map<string, int> registerEncode;
		map<int, string> registerDecode;
		map<int, string> opcodeDecode;
		map<string, opcode> opcodeEnumDecode;
		Decoder();

};

#endif
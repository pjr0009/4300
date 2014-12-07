#include "../include/decoder.h"


Decoder::Decoder(){
	registerEncode["$0"]   = 00000;
	registerEncode["$1"]   = 00001;
	registerEncode["$2"]   = 00010;
	registerEncode["$3"]   = 00011;
	registerEncode["$4"]   = 00100;
	registerEncode["$5"]   = 00101;
	registerEncode["$6"]   = 00110;
	registerEncode["$7"]   = 00111;
	registerEncode["$8"]   = 01000;
	registerEncode["$9"]   = 01001;
	registerEncode["$10"]   = 01010;
	registerEncode["$11"]   = 01011;
	registerEncode["$12"]   = 01100;
	registerEncode["$13"]   = 01101;
	registerEncode["$14"]   = 01110;
	registerEncode["$15"]   = 01111;
	registerEncode["$f0"]	= 10000;
	registerEncode["$f1"]	= 10001;
	registerEncode["$f2"]   = 10010;
	registerEncode["$f3"]   = 10011;
	registerEncode["$f4"]   = 10100;
	registerEncode["$f5"]   = 10101;
	registerEncode["$f6"]   = 10110;
	registerEncode["$f7"]   = 10111;
	registerEncode["$f8"]   = 11000;
	registerEncode["$f9"]   = 11001;
	registerEncode["$f10"]   = 11010;
	registerEncode["$f11"]   = 11011;
	registerEncode["$f12"]   = 11100;
	registerEncode["$f13"]   = 11101;
	registerEncode["$f14"]   = 11110;
	registerEncode["$f15"]   = 11111;

	registerDecode [00000] = "$0";
	registerDecode [00001] = "$1"; 
	registerDecode [00010] = "$2"; 
	registerDecode [00011] = "$3"; 
	registerDecode [00100] = "$4"; 
	registerDecode [00101] = "$5"; 
	registerDecode [00110] = "$6"; 
	registerDecode [00111] = "$7"; 
	registerDecode [01000] = "$8"; 
	registerDecode [01001] = "$9"; 
	registerDecode [01010] = "$10"; 
	registerDecode [01011] = "$11"; 
	registerDecode [01100] = "$12"; 
	registerDecode [01101] = "$13"; 
	registerDecode [01110] = "$14"; 
	registerDecode [01111] = "$15"; 
	registerDecode [10000] = "$f0";
	registerDecode [10001] = "$f1"; 
	registerDecode [10010] = "$f2"; 
	registerDecode [10011] = "$f3"; 
	registerDecode [10100] = "$f4"; 
	registerDecode [10101] = "$f5"; 
	registerDecode [10110] = "$f6"; 
	registerDecode [10111] = "$f7"; 
	registerDecode [11000] = "$f8"; 
	registerDecode [11001] = "$f9"; 
	registerDecode [11010] = "$f10"; 
	registerDecode [11011] = "$f11"; 
	registerDecode [11100] = "$f12"; 
	registerDecode [11101] = "$f13"; 
	registerDecode [11110] = "$f14"; 
	registerDecode [11111] = "$f15"; 

	opcodeDecode[00000] = "addi";
	opcodeDecode[00001] = "subi"; 
	opcodeDecode[00010] = "li";
	opcodeDecode[00011] = "lb"; 
	opcodeDecode[00100] = "b";
	opcodeDecode[00101] = "bge"; 
	opcodeDecode[00110] = "add";
	opcodeDecode[00111] = "beqz";
	opcodeDecode[01000] = "syscall";
	opcodeDecode[01001] = "bne";
	opcodeDecode[01011] = "la";
	opcodeDecode[01100] = "ld";
	opcodeDecode[01101] = "sd";
	opcodeDecode[01110] = "fadd";
	opcodeDecode[01111] = "fsub";
	opcodeDecode[10000] = "fmul";
	opcodeDecode[10001] = "lid";
	opcodeDecode[11111] = "nop";

	opcodeEnumDecode["addi"] = ADDI;
	opcodeEnumDecode["subi"] = SUBI; 
	opcodeEnumDecode["li"] = LI;
	opcodeEnumDecode["lb"] = LB; 
	opcodeEnumDecode["b"] = B;
	opcodeEnumDecode["bge"] = BGE; 
	opcodeEnumDecode["add"] = ADD;
	opcodeEnumDecode["beqz"] = BEQZ;
	opcodeEnumDecode["syscall"] = SYSCALL;
	opcodeEnumDecode["bne"] = BNE;
	opcodeEnumDecode["la"] = LA;
	opcodeEnumDecode["nop"] = NOP;
	opcodeEnumDecode["ld"] = LD;
	opcodeEnumDecode["sd"] = SD;
	opcodeEnumDecode["fadd"] = FADD;
	opcodeEnumDecode["fsub"] = FSUB;
	opcodeEnumDecode["fmul"] = FMUL;
	opcodeEnumDecode["lid"] = LID;


	
}

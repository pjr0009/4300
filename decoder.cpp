#include "decoder.h"


Decoder::Decoder(){
	registerEncode["zero"]  = 00000;
	registerEncode["$at"]   = 00001;
	registerEncode["$v0"]   = 00010;
	registerEncode["$v1"]   = 00011;
	registerEncode["$a0"]   = 00100;
	registerEncode["$a1"]   = 00101;
	registerEncode["$a2"]   = 00110;
	registerEncode["$a3"]   = 00111;
	registerEncode["$t0"]   = 01000;
	registerEncode["$t1"]   = 01001;
	registerEncode["$t2"]   = 01010;
	registerEncode["$t3"]   = 01011;
	registerEncode["$t3"]   = 01100;
	registerEncode["$t4"]   = 01101;
	registerEncode["$t5"]   = 01110;
	registerEncode["$t6"]   = 01111;
	registerEncode["$t7"]   = 10000;

	registerDecode [00000] = "zero";
	registerDecode [00001] = "$at"; 
	registerDecode [00010] = "$v0"; 
	registerDecode [00011] = "$v1"; 
	registerDecode [00100] = "$a0"; 
	registerDecode [00101] = "$a1"; 
	registerDecode [00110] = "$a2"; 
	registerDecode [00111] = "$a3"; 
	registerDecode [01000] = "$t0"; 
	registerDecode [01001] = "$t1"; 
	registerDecode [01010] = "$t2"; 
	registerDecode [01011] = "$t3"; 
	registerDecode [01100] = "$t3"; 
	registerDecode [01101] = "$t4"; 
	registerDecode [01110] = "$t5"; 
	registerDecode [01111] = "$t6"; 
	registerDecode [10000] = "$t7";

	opcodeDecode[00000] = "addi";
	opcodeDecode[00001] = "subi"; 
	opcodeDecode[00010] = "li";
	opcodeDecode[00011] = "lb"; 
	opcodeDecode[00100] = "b";
	opcodeDecode[00101] = "bge"; 





	
}

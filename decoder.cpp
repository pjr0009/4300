#include "decoder.h"


Decoder::Decoder(){
	registerEncode["zero"]  = 00000;
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
	registerEncode["$16"]   = 10000;

	registerDecode [00000] = "zero";
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
	registerDecode [10000] = "$16";

	opcodeDecode[00000] = "addi";
	opcodeDecode[00001] = "subi"; 
	opcodeDecode[00010] = "li";
	opcodeDecode[00011] = "lb"; 
	opcodeDecode[00100] = "b";
	opcodeDecode[00101] = "bge"; 





	
}

#ifndef _DECODER_H
#define _DECODER_H

#include <string.h>
#include <iostream>
#include <map>
#include <utility>

using namespace std;

class Decoder {
	public:
		map<string, int> registerEncode;
		map<int, string> registerDecode;
		Decoder();

};

#endif
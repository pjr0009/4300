all: 
	gcc -g -lstdc++ instruction.cpp decoder.cpp loader.cpp register_file.cpp main.cpp -o main

	
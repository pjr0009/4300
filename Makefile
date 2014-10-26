all: 
	gcc -g -lstdc++ instruction.cpp stages.cpp data_path.cpp decoder.cpp loader.cpp register_file.cpp main.cpp -o main

	
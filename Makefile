all: 
	g++ -std=c++11 -g src/instruction.cpp src/stages.cpp src/data_path.cpp src/decoder.cpp src/loader.cpp src/register_file.cpp src/main.cpp src/scoreboard.cpp -o pipeSim

	

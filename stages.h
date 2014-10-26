#include "instruction.h"
#include "data_path.h"
#include <stdio.h>


void if_stage(DataPath *data_path);
void id_stage(DataPath *data_path);
void execute_stage(DataPath *data_path);
void wb_stage(DataPath *data_path);


void if_debug(DataPath data_path);
void id_debug(DataPath data_path);

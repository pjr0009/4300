#include "instruction.h"
#include "data_path.h"
#include "pipeline.h"
#include <stdio.h>

void id1_stage(DataPath *data_path);
void id2_stage(DataPath *data_path);
void execute_stage(DataPath *data_path);
void wb_stage(DataPath *data_path, int* count);


// need updating
// void if_debug(DataPath data_path, if_id_latch *if_id);
// void id_debug(DataPath data_path, id_ex_latch *id_ex);
// void wb_debug(DataPath data_path, mem_wb_latch *mem_wb);
// void memory_stage(DataPath *data_path, ex_mem_latch *ex_mem, mem_wb_latch *mem_wb);



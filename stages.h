#include "instruction.h"
#include "data_path.h"
#include "pipeline.h"
#include <stdio.h>


void if_stage(DataPath *data_path, if_id_latch *if_id);
void id_stage(DataPath *data_path, if_id_latch *if_id, id_ex_latch *id_ex);
void execute_stage(DataPath *data_path, id_ex_latch *id_ex, ex_mem_latch *ex_mem);
void wb_stage(DataPath *data_path, mem_wb_latch *mem_wb, int* count);


void if_debug(DataPath data_path, if_id_latch *if_id);
void id_debug(DataPath data_path, id_ex_latch *id_ex);
void wb_debug(DataPath data_path, mem_wb_latch *mem_wb);
void memory_stage(DataPath *data_path, ex_mem_latch *ex_mem, mem_wb_latch *mem_wb);



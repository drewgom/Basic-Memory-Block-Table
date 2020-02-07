#include "OSstructures.h"
#include <stdlib.h>
#include <time.h> 


void add_new_process(queue &queue_ref, memory_block_table &MBT);
void show_mem_block_state(queue &queue_ref, memory_block_table &MBT);
void terminate_process(queue &queue_ref, memory_block_table &MBT);

const int SIZE_OF_MEMORY = 512;

// Name: Andrew Gomez
// Class: CECS 326
// Assignment: Assignment 1
// Due Date: Feb 11, 2020
// main.h

#include "OSstructures.h"
#include <stdlib.h>
#include <time.h> 
#include <ctype.h>
#include <iomanip>
#include <limits>


void add_new_process(queue &queue_ref, memory_block_table &MBT);
void show_mem_block_state(queue &queue_ref, memory_block_table &MBT);
void terminate_process(queue &queue_ref, memory_block_table &MBT, int PID, bool display_error_message);
void display_process(process_control_block* process);
void exit_sequence(queue &queue_ref, memory_block_table &MBT);
process_control_block* unqueue_process(queue &queue_ref, memory_block_table &MBT, int PID);

const int SIZE_OF_MEMORY = 512;

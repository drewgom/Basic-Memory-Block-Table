#include <iostream>
#include <iomanip>
#include "main.h"

using namespace std;


int main()	{
	// Creates the two data structures that we use
	memory_block_table mbt(SIZE_OF_MEMORY);
	queue ready_queue;

	// Initializes the random seed that we will use later to generate random numbers
	srand(time(NULL));


	// Runs the menu
	cout << "CECS 326 ASSIGNMENT 1" << endl;
	while (true)	{
		int choice;
		cout << "Please select a menu option: " << endl;
		cout << "1) Add a new process to the system" << endl;
		cout << "2) Print the state of the system" << endl;
		cout << "3) Terminate a process by PID" << endl;
		cout << "4) Exit" << endl;
		cin >> choice;
		switch(choice)	{
			case 1:
				add_new_process(ready_queue, mbt);
				break;
			case 2:
				show_mem_block_state(ready_queue, mbt);
				break;
			case 3:
				terminate_process(ready_queue, mbt);
				break;
			case 4:
				exit(0);
			default:
				cout << "Invalid Selection. Please try again" << endl;
				break;
		}



		cout << endl;
	}



	return 0;
}



void add_new_process(queue &queue_ref, memory_block_table &MBT)	{
	cout << "Adding new a process . . ." << endl;
	// Generates a random number between 25 and 120 for the size of the new process.
	// I took the modulus of the random number and 96 to get a number between 0 and 95. Then, I added that number by 25 to get the size our our block.
	int new_block_size = rand() % 96 + 25;

	if (new_block_size <= MBT.num_open_blocks)	{
		process_control_block* new_process = new process_control_block;

		// Assigns the process size to the process control block, and initializes the page table
		new_process->process_size = new_block_size;
		new_process->page_table = new int[new_process->process_size];

		// Once we have our new process control block allocated, we need to "assign the process's data" to the memory block table. 
		// We will do this by marking the boolean for block's number as true, then
		int next_page_index = 0;

		// This will iterate while we still have not assigned all the blocks
		while (0 < new_block_size)	{

			// Selects a potential block address for the new block
			int potential_new_block_num = rand() % MBT.size;

			// If the block at the potental block address is not already occupied, then we should allocate one block of our new process to that spot in the table
			if (!MBT.block_status[potential_new_block_num])	{
				// First we start by marking the block as occupied, and decrementing the number of remaining blocks
				MBT.block_status[potential_new_block_num] = true;
				MBT.num_open_blocks--;

				// We then decrement the variable that tells us the number of blocks from our process that we still need to assign
				new_block_size--;

				// We then update the page table with the memory block address
				new_process->page_table[next_page_index] = potential_new_block_num;
				next_page_index++;
			}
		}

		// Once the page is accounted for in the memory block table, we need to add it to the ready queue
		queue_ref.push(*new_process);


		// Once we are done, we will give the user some information about the process that was added 
		cout << "Allocated a process of size " << new_process->process_size << " memory blocks" << endl;

	}
	else	{
		cout << "There is currently not enough memory to handle a process of size " << new_block_size << " memory blocks (only ";
		cout << MBT.num_open_blocks << " memory blocks are available)." << endl; 
	}
}


void show_mem_block_state(queue &queue_ref, memory_block_table &MBT)	{
	// The first step in displaying the system's state is to show which memory blocks are free.
	cout << "CURRENT STATE: " << MBT.size - MBT.num_open_blocks << " blocks occupied, " << MBT.num_open_blocks << " blocks free." <<endl;
	for (int i = 0; i < MBT.size; i++)	cout << MBT.block_status[i];
	cout << endl;

	// After we have shown the state of the memory block table, we need to show the state of each process in the ready queue.
	// We will do this by displaying the process ID, the number of blocks allocated, and the page table of the process.
	// To do this, we will have to iterate over the ready queue.
	
	
}

void terminate_process(queue &queue_ref, memory_block_table &MBT)	{

}



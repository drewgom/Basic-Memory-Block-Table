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
				int pid;
				cout << "Please enter the process ID of the process you would like to terminate: " << endl;
				cin >> pid;
				cin.clear();
				terminate_process(ready_queue, mbt, pid);
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
	
	for (int i = 0; i < MBT.size; i++)	{
		cout << MBT.block_status[i];
		// I wanted this table to look nice - so, I made it be 8x64 table. created a new line after every 64th bit.
		if ((i+1) % 64 == 0) cout << endl;
		else if ((i+1) % 8 == 0) cout << " "; 
	}

	// After we have shown the state of the memory block table, we need to show the state of each process in the ready queue.
	// We will do this by displaying the process ID, the number of blocks allocated, and the page table of the process.
	// To do this, we will have to iterate over the ready queue.
	
	pcb_queue_node* current = queue_ref.front;

	cout << "CURRENT PROCESSES: " << endl; 
	if (current == NULL) cout << "There are no active processes on the system besides the OS." << endl;
	else	{
		while (current != NULL)	{
			display_process(current->_process);
			current = current->next;
			cout << endl;
		}
	}

	
}

void terminate_process(queue &queue_ref, memory_block_table &MBT, int PID)	{
		// This method will remove the process from the ready queue
		process_control_block* terminatee = unqueue_process(queue_ref, MBT, PID);

		if (terminatee != NULL)	{
			// To free up the memory in the memory block table, we need to iterate through the page
			// table and then mark every memory block in our page table as free

			for (int i = 0; i < terminatee->process_size; i++)	{
				MBT.block_status[terminatee->page_table[i]] = false;
				MBT.num_open_blocks++;
			}

			delete terminatee->page_table;

		}
}



void display_process(process_control_block* process)	{
		// There are three things we should display:
		//		1. The PID
		//		2. The size of the process
		// 		3. The page table

		cout << "PROCESS ID (PID):" << process->process_ID << endl;
		cout << "PROCESS SIZE:" << process->process_size << endl;
		cout << "PAGE TABLE:" << endl;

		int last_value_displayed = 0;

		// This for loop will display every index in the page table. If we have displayed 16 indices in a row, 
		// we want to move on to the next line. However, before this can be done, we have to display the contents
		// at each element in the page table up to that point
		for (int i = 0; i < process->process_size; i++)	{
			cout << setw(4) << i << " ";
			// If we get to the 16th spot, we need to add in all the values in the table up to that point.
			if ((i+1) % 16 == 0) {
				cout << endl;
				while (last_value_displayed <= i)	{
					cout << setw(4) << process->page_table[last_value_displayed] << " ";
					last_value_displayed++;
				}
				cout << endl << endl;

			}

		}
		cout << endl;

		// If our table's size was not a multiple of 16, then there are still some values that have not been displayed.
		// This code is added to ensure that the rest of the contents get displayed.
		if (last_value_displayed < process->process_size-1)	{
				while (last_value_displayed < process->process_size)	{
					cout << setw(4) << process->page_table[last_value_displayed] << " ";
					last_value_displayed++;
				}
		}
}

process_control_block* unqueue_process(queue &queue_ref, memory_block_table &MBT, int PID)	{
	// First, we need to find the process that is to be removed
	pcb_queue_node* current = queue_ref.front;

	while (current != NULL) {
		if (current->_process->process_ID == PID) break;
		else current = current->next;
	}

	// When the while loop is done, there is one of two scenarios:
	//	1. We located the process (current does not equal NULL)
	// 	2. We did not locate the process (current is NULL)

	if (current == NULL)	{
		cout << "The process you are trying to terminate is not in the system." << endl;
		return NULL;
	} 

	else	{
		// When we need to terminate a process, we need to:
		// 	1. Remove the process from the queue
		//	2. Free up memory in the memory block table
		// 	3. Deallocate the memory from the process

		// To remove the process from the queue, we need to make the previous element in the queue's next value be the current element's next value.
		// There are three special case for this:
		//		1. If we only have one element
		// 		1. If we are trying to delete the front element 
		//		2. If we are trying to delete the back element
		// To solve this, we will check first for a single element. If there is more that one element, 
		// We will check for either of the last two cases. If that is so, we reassign these positions.

		// Case 1: There is one element 

		cout << "Found current: ID is " << current->_process->process_ID << endl;
		if (current == queue_ref.front && current == queue_ref.back)	{
			queue_ref.front = NULL;
			queue_ref.back = NULL;
		}
		else {

			if (current != queue_ref.front && current != queue_ref.back) {
				current->previous->next = current->next;
				current->next->previous = current->previous;
			}

			// Case 2: We are deleting the front
			else if (current == queue_ref.front) {
				queue_ref.front = queue_ref.front->next;
				queue_ref.front->previous = NULL;
			}
	
			// Case 3: We are deleting the back
			else if (current == queue_ref.back)	{

				queue_ref.back = queue_ref.back->previous;
				queue_ref.back->next = NULL;
			}
			
		}
	}

	return current->_process;
}



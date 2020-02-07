// The Memory Block Table
struct memory_block_table	{
	int size;
	int num_open_blocks;
	bool* block_status;

	memory_block_table(int _size):size(_size),num_open_blocks(_size)	{
		// Declares the array that tracks the occupied blocks
		block_status = new bool[size];

		// Sets the first 32 booleans to 'ON' - to represent the operating system taking up the first 32 memory blocks
		for (int i = 0; i < size; i++) {
			if (i < 32)	{
				block_status[i] = true;
				num_open_blocks--;
			}
			else block_status[i] = false;
		}
	}
};

// The structure for a process control block
struct process_control_block {
	static int LAST_USED_ID;
	int process_ID;
	int process_size;
	int* page_table;
	
	process_control_block():process_ID(LAST_USED_ID++)	{}
};

// A structure that allows us to use processes in our queue structure
struct pcb_queue_node			{
	process_control_block* _process;
	pcb_queue_node* next;
	pcb_queue_node* previous;

	pcb_queue_node(process_control_block &temp_process):_process(&temp_process),next(NULL),previous(NULL) {}

};

// A queue that stores processes 
struct queue						{
	pcb_queue_node* front;
	pcb_queue_node* back;
	
	queue():front(NULL),back(NULL) {};

	void push(process_control_block &temp_process)	{
		pcb_queue_node* new_pcb_node = new pcb_queue_node(temp_process);
		if(back == NULL)	{
			front = new_pcb_node;
			back = new_pcb_node;
		}
		else	{
			new_pcb_node->previous = back;
			back->next = new_pcb_node;
			back = new_pcb_node;
		}
	}

	void pop()	{
		if (front != NULL && front != back) front = front->next;
		else if (front == back)	{
			front = NULL;
			back = NULL;
		}
	}
};

int process_control_block::LAST_USED_ID = 1;
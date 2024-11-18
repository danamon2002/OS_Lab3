#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "MMS.h"

/* Constant/datatype defs below vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */

const int MAX_SIZE = 30*1024; // maximum memory allocation, in bytes
const int NUM_BLOCKS = 30;

/* Function defs below */

void *memory_management(void *arg){
	int num = *(int *)arg; // Cast and dereference the argument
	printf("Memory manager running on thread %d.\n", num);
	
	// function body: Allocate memory (in kilobytes) based on max_size arg.
	void *sys_mem = malloc(MAX_SIZE);
	
	for(int blocks = 0; blocks < NUM_BLOCKS; blocks++){
		//make memory blocks
	}
	printf("Allocating %d bytes in memory...\n", MAX_SIZE);
	if (!sys_mem){
		perror("Memory Allocation failed.");
	}
	int block_size = MAX_SIZE / NUM_BLOCKS;
	
	free(arg); // Free the dynamically allocated memory
	free(sys_mem); // free fixed allocated memory.
	return NULL;
}

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include "MMS.h"

/* Constant/datatype defs below vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */

const int MAX_SIZE = 30*1024; // maximum memory allocation, in bytes
const int NUM_BLOCKS = 10;

typedef struct MemoryBlock { // TODO segment memory into MemoryBlock segments.
	void *start;
	// end = start + block_size
	size_t size;
	bool is_free;
	struct MemoryBlock *next; // next block if needed.
} MemoryBlock;

/* Function defs below */

void *memory_management(void *arg){
	int num = *(int *)arg; // Cast and dereference the argument
	printf("Memory manager running on thread %d.\n", num);
	
	// function body: Allocate memory (in kilobytes) based on max_size arg.
	//void *sys_mem = malloc(MAX_SIZE);
	
	MemoryBlock *blocks[NUM_BLOCKS]; // make array of blocks.
	int block_size = MAX_SIZE / NUM_BLOCKS; // TODO delete?
	
	for (int block = 0; block < NUM_BLOCKS; block++) {
		blocks[block] = (MemoryBlock *)malloc(sizeof(MemoryBlock));
		if (blocks[block] == NULL) {
		    fprintf(stderr, "Error: Failed to allocate memory for block %d\n", block);
		    return; // Exit on allocation failure
		}

		// Allocate memory for the block's data
		blocks[block]->start = malloc(MAX_SIZE / NUM_BLOCKS);
		if (blocks[block]->start == NULL) {
		    fprintf(stderr, "Error: Failed to allocate memory for block %d's data\n", block);
		    free(blocks[block]); // Free the MemoryBlock itself
		    return; // Exit on allocation failure
		}

		// Initialize the rest of the fields
		blocks[block]->size = MAX_SIZE / NUM_BLOCKS;
		blocks[block]->is_free = true;
		blocks[block]->next = NULL;

		printf("Allocated block %d: Size = %zu bytes\n", block, blocks[block]->size);
	}
	
	free(arg); // Free the dynamically allocated memory
	 // Free the blocks
    for (int block = 0; block < NUM_BLOCKS; block++) {
        free(blocks[block]->start); // Free the memory allocated for the block's data
        free(blocks[block]);        // Free the MemoryBlock itself
    }
	return;
}

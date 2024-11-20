//#include "MMS.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define MAX_SIZE (30 * 1024) // 30MB
#define MAX_BLOCKS 10 // Maximum number of blocks in memory.

struct DataBlock {
	int id;
	int in_use;
	void *blockStart;
	void *blockEnd;
} DataBlock;

//Single threaded memory manager test.
//Dana Maloney

void *start = NULL;
void *end = NULL;

struct DataBlock memoryTable[MAX_BLOCKS]; // array of data blocks.

int block_size(struct DataBlock *block){
/* Calculates size of a DataBlock. */
	return (char *)block->blockEnd - (char *)block->blockStart;
}

void initialize_memory(){
	/* Initialize memory for the MMS. */
	start = malloc(MAX_SIZE);
	end = (void *)((char *)start + MAX_SIZE); // use char pointer because it moves 1 byte at a time.
	printf("Memory starts at %p\n", start);
	printf("Memory ends at %p\n", end);
}

void *first_fit(int size, int id){
	/* First fit block placement function. */
	
	if(size > 0 && (size & (size - 1)) == 0){ //check if chunk is power of 2: Will happen if only 1 bit is set.
		printf("Number is power of 2\n");

		for(int i = 0; i < MAX_BLOCKS; i++){ //look for first available block.
			printf("Current Block: %d\n", i);
			if (memoryTable[i].in_use == 0){ // check for open block
				printf("OPEN BLOCK AT INDEX %d\n", i);
				//TODO run checks for if there is enough space for block.
				return NULL;
			} else {
				printf("Block %d is taken.\n", i);
			}
		}
		//if you get here, it means you went through all the blocks and there was no room.
		return NULL;
	} else {
		printf("Number is not power of 2\n");
		return NULL;
	}
}

void free_memory(){
	/* Free all allocated resources. */
	free(start);
}

int main(){
	for(int i = 0; i < 10; i++){
		break;
	}
	initialize_memory();
	first_fit(4, 21);
	first_fit(19, 1);
	free_memory();
	return 0;
}

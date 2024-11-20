//#include "MMS.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define MAX_SIZE (30 * 1024) // 30MB
#define MAX_BLOCKS 10 // Maximum number of blocks in memory.

struct DataBlock {
	int id;
	int is_empty;
	void *start;
	void *end;
}

//Single threaded memory manager test.
//Dana Maloney

void *start = NULL;
void *end = NULL;
struct DataBlock memoryTable[MAX_BLOCKS]; // array of data blocks.

int block_size(struct DataBlock *block){
/* Calculates size of a DataBlock. */
	return (char *)block->end - (char *)block->start;
}

void initialize_memory(){
	/* Initialize memory for the MMS. */
	start = malloc(MAX_SIZE);
	end = (void *)((char *)start + MAX_SIZE); // use char pointer because it moves 1 byte at a time.
	printf("Memory starts at %p\n", start);
	printf("Memory ends at %p\n", end);
}

void *first_fit(int size, int id){

	void *blockStart = start;
	void *blockEnd = blockStart;

	//check if chunk is power of 2: Will happen if only 1 bit is set.
	if(size > 0 && (size & (size - 1)) == 0){
		printf("Number is power of 2\n");
		//look for first available block.
		for(int i = 0; i < MAX_BLOCKS; i++){
			printf("Current Block: %d\n", memoryTable[0][i]);
			if (memoryTable[0][i] == 0){ // check for open block
				blockStart = (void *)((char *)blockStart + size); // move blockStart to the start of the next block.
				if (blockStart > memoryTable[1][i]) {
				
				}
				memoryTable[0][i] = id;
				memoryTable[1][i] = size;
				if (((void *)((char *)blockStart + size ) < end){ // make sure that there's room for the block.
					print("Yippeeeee!!\n");
				}
				printf("Wrote block for thread %d of size %d in block %d\n", id, size, i);
				return 
			} else {
				blockStart = (void *)((char *)blockStart + size); 
			}
		}
		//put memory in.
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

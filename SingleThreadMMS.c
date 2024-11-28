#include <stdio.h>
#include <pthread.h>
#include "MMS.h"

<<<<<<< Updated upstream:SingleThreadMMS.c
#define MAX_SIZE (30 * 1024) // 30MB
#define MAX_BLOCKS 10 // Maximum number of blocks in memory.

struct DataBlock {
	int id;
	int in_use;
	void *block_start;
	void *block_end;
} DataBlock;
=======
void* mms_thread(void* arg) {
    printf("Creating Thread: MMS\n");
    //long* blocks = (long*)arg;

    // Loops until all user-requested jobs are done
    while (jobsDone < userNum) {
        // Check `userBuffer` for job requests
        for (int f = 0; f < BUFFER_SIZE; f++) {
            // If request at index is greater than 0
            if (userBuffer[f] > 0) {
                // Lock mutex for safety
                pthread_mutex_lock(&mutx);

                // Check to see if requested block is taken
                for (int g = 0; g < BUFFER_SIZE; g++) {
                    if (userBuffer[f] == bp_buffer[g] && f != g) {
                        // Free up memory by removing a job
                        bp_buffer[g] = 0;
>>>>>>> Stashed changes:MMS.c

                        // Allocate the requested memory block
                        bp_buffer[f] = userBuffer[f];
                    } else if (userBuffer[f] != bp_buffer[g] || f != g) {
                        bp_buffer[f] = userBuffer[f];
                    }
                }
                sem_post(&bin_sem);        // Increase semaphore
                pthread_mutex_unlock(&mutx); // Unlock mutex
            }
        }

        // Reset `index_counter` when it reaches BUFFER_SIZE
        if (index_counter >= BUFFER_SIZE) {
            index_counter = 0;
        }
    }
<<<<<<< Updated upstream:SingleThreadMMS.c
}

void initialize_memory(){
	/* Initialize memory for the MMS. */
	start = malloc(MAX_SIZE);
	end = (void *)((char *)start + MAX_SIZE); // use char pointer because it moves 1 byte at a time.
	printf("Memory starts at %p\n", start);
	printf("Memory ends at %p\n", end);
	printf("Memory available: %lu bytes\n", (uintptr_t)end - (uintptr_t)start);
}

void *block_start(int index){
	/* finds start of current block. */
	
	//printf("\n!!!!!!!!!!!Finding start for block at index %d !!!!!!!!!!!!!\n", index);
	void *result = NULL;
	
	for(int i = index; i >= 0; i--){
		if(i - 1 < 0){
			result = start;
			//printf("Result is start.\n");
			break;
		} else { // if you're not in the first block, look at the end of the block before you.
			if(memoryTable[i - 1].in_use != 0){
				//printf("Block %d is in use.\n", i - 1);
				result = memoryTable[i - 1].block_end + 1;
				//printf("i = %d, result = %lu\n", i, result);
				break;
			} else {
				//printf("Block %d isn't in use.\n", i - 1);
			}
		}
	}
	
	//printf("Block %d's start address is %lu\n", index, (uintptr_t)result);
	//printf("Start is equal to %lu\n\n", (uintptr_t)start);
	return result;
}

void *block_max_end(int index){
	/* finds max possible end of current block. */
	
	//printf("\nFinding max end for block at index %d\n", index);
	
	void *result = NULL;
	
	for(int i = index + 1; i < MAX_BLOCKS; i++){// work forwards to find end.
		if(i >= MAX_BLOCKS - 1){// if in the last block, max end is end of memory.
			result = end;
			//printf("Result is end.\n");
		}
		else if(memoryTable[i].in_use != 0){
			result = memoryTable[i].block_start - 1;
			//printf("Result from block %d and is %lu\n", i, (uintptr_t)result);
		}
	}	
	//printf("Block %d's max end address is %lu\n", index, (uintptr_t)result);
	//printf("End is equal to %lu\n\n", (uintptr_t)end);
	return result;
}

uintptr_t available_space(int index){
	/* return, in bytes, the amount of space available at an index. */

	void *space_start = start;
	void *space_end = end;
	
	space_end = block_max_end(index);
	space_start = block_start(index);

	uintptr_t space = (uintptr_t)space_end - (uintptr_t)space_start;
	//printf("%lu - %lu = %lu\n", (uintptr_t)space_end, (uintptr_t)space_start, space);
	return space;
}

void *first_fit(int size, int id){
	/* First fit block placement function. */
	
	if(size > 0 && (size & (size - 1)) == 0){ //check if chunk is power of 2: Will happen if only 1 bit is set.
		//printf("Number is power of 2\n");
		for(int i = 0; i < MAX_BLOCKS; i++){ //look for first available block.
			printf("Current Block: %d\n", i);
			
			if (memoryTable[i].in_use == 0){ // check for open block
				printf("OPEN BLOCK AT INDEX %d\n", i);
				
				// Run checks for if there is enough space for block.
				printf("Check space for block at index %d\n", i);
				if (size <= available_space(i)){  // enough space
					//put block at this index!
					memoryTable[i].id = id;
					memoryTable[i].in_use = 1;
					memoryTable[i].block_start = block_start(i);
					memoryTable[i].block_end = block_start(i) + size;
					return NULL; // TODO return success, space granted
				} else {
					printf("No room for block of size %d: Only <=%lu\n", size, available_space(i));
					return NULL; // TODO return no space
				}
			} else {
				printf("Block %d is taken.\n", i);
			}
		}
		return NULL;
	} else {
		printf("Requested %d bytes is not power of 2\n", size);
		return NULL; // return improper request
	}
}

void free_block(int id){
	/* free up previously allocated block with matching id. */
	for(int i = 0; i < MAX_BLOCKS; i++){
		if (memoryTable[i].id == id){
			printf("Freeing block %d with ID %d\n", i, memoryTable[i].id);
			memoryTable[i].id = 0;
			memoryTable[i].in_use = 0;
			memoryTable[i].block_start = NULL;
			memoryTable[i].block_end = NULL;
			//free(&memoryTable[i]);
		}
	}
}

void free_memory(){
	/* Free all allocated resources. */
	free(start);
}

int main(){
	initialize_memory();
	first_fit(4, 1);
	first_fit(1024, 2);
	first_fit(64, 3);
	first_fit(19, 4); // try block with size that isn't 2^n
	free_block(2); // free up block
	first_fit(256, 5); // should go into freed block.
	first_fit(32768, 6); // try block too large to be placed.
	for(int i = 7; i < 20; i++){
		first_fit(256, i);
	}

	//print list of all blocks.
	for(int i = 0; i < MAX_BLOCKS; i++){
		print_data_block(&memoryTable[i]);
	}
	free_memory();
	return 0;
=======
    return NULL;
>>>>>>> Stashed changes:MMS.c
}

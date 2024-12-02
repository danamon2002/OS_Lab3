#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include "MMS.h"

//Dana Maloney

void *start = NULL;
void *end = NULL;
int buff_index = 0;

DataBlock memoryTable[MAX_BLOCKS]; // array of data blocks.

void print_data_block(DataBlock *block) {
    if (block == NULL) {
        printf("Invalid DataBlock (NULL pointer).\n");
        return;
    }

    printf("\033[34mDataBlock Information:\033[0m\n");
    printf("\033[34mID: %d\033[0m\n", block->id);
    printf("\033[34mIn Use: %s\033[0m\n", block->in_use ? "Yes" : "No");
    printf("\033[34mBlock Start Address: %p\033[0m\n", block->block_start);
    printf("\033[34mBlock End Address: %p\033[0m\n", block->block_end);

    // Calculate and print the size of the block
    if (block->block_start != NULL && block->block_end != NULL) {
        printf("\033[34mBlock Size: %ld bytes\033[0m\n", (char *)block->block_end - (char *)block->block_start);
    } else {
        printf("\033[34mBlock Size: Unknown (start or end is NULL)\033[0m\n");
    }
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

int first_fit(int size, int id){
	/* First fit block placement function. */
	
	if(size > 0 && (size & (size - 1)) == 0){ //check if chunk is power of 2: Will happen if only 1 bit is set.
		//printf("Number is power of 2\n");
		for(int i = 0; i < MAX_BLOCKS; i++){ //look for first available block.
			printf("Current Block: %d\n", i);
			
			if (memoryTable[i].in_use == 0){ // check for open block
				printf("OPEN BLOCK AT INDEX %d\n", i);
				
				// Run checks for if there is enough space for block.
				printf("Check space for block at index %d\n", i);
				if ((uintptr_t)size <= available_space(i)){  // enough space
					//put block at this index!
					memoryTable[i].id = id;
					memoryTable[i].in_use = 1;
					memoryTable[i].block_start = block_start(i);
					memoryTable[i].block_end = block_start(i) + size;
					return 1; // return success, space granted
				} else {
					printf("No room for block of size %d: Only <=%lu\n", size, available_space(i));
					return -1; // return no space
				}
			} else {
				printf("Block %d is taken.\n", i);
			}
		}
		return 1;
	} else {
		printf("Requested %d bytes is not power of 2\n", size);
		return -2; // return invalid request
	}
}

int best_fit(int size, int id) {
    /* Best fit block placement function. (Look for fit closest to requested size) */

    if (size > 0 && (size & (size - 1)) == 0) { // Check if chunk is a power of 2
        int best_index = -1;
        uintptr_t min_diff = (uintptr_t)-1; // Start with maximum possible difference

        // Iterate over all blocks to find the best fit
        for (int i = 0; i < MAX_BLOCKS; i++) {
            printf("Current Block: %d\n", i);

            if (memoryTable[i].in_use == 0) { // Check for open block
                printf("OPEN BLOCK AT INDEX %d\n", i);

                // Check if there is enough space for the block
                uintptr_t available = available_space(i);
                if ((uintptr_t)size <= available) {
                    uintptr_t diff = available - (uintptr_t)size;
                    if (diff < min_diff) { // Check if this is a better fit
                        min_diff = diff;
                        best_index = i;
                    }
                }
            } else {
                printf("Block %d is taken.\n", i);
            }
        }

        if (best_index != -1) { // A suitable block was found
            printf("Best block found at index %d\n", best_index);

            // Allocate the block
            memoryTable[best_index].id = id;
            memoryTable[best_index].in_use = 1;
            memoryTable[best_index].block_start = block_start(best_index);
            memoryTable[best_index].block_end = block_start(best_index) + size;
            return 1; // Return success
        } else {
            printf("No suitable block found for size %d\n", size);
            return -1; // Return no space
        }
    } else {
        printf("Requested %d bytes is not power of 2\n", size);
        return -2; // Return invalid request
    }
}

int worst_fit(int size, int id) {
    /* Worst fit block placement function. (Look for largest space possible) */

    if (size > 0 && (size & (size - 1)) == 0) { // Check if chunk is a power of 2
        int worst_index = -1;
        uintptr_t max_space = 0; // Start with no space

        // Iterate over all blocks to find the worst fit
        for (int i = 0; i < MAX_BLOCKS; i++) {
            printf("Current Block: %d\n", i);

            if (memoryTable[i].in_use == 0) { // Check for open block
                printf("OPEN BLOCK AT INDEX %d\n", i);

                // Check if there is enough space for the block
                uintptr_t available = available_space(i);
                if ((uintptr_t)size <= available) {
                    if (available > max_space) { // Check if this is a worse fit
                        max_space = available;
                        worst_index = i;
                    }
                }
            } else {
                printf("Block %d is taken.\n", i);
            }
        }

        if (worst_index != -1) { // A suitable block was found
            printf("Worst block found at index %d\n", worst_index);

            // Allocate the block
            memoryTable[worst_index].id = id;
            memoryTable[worst_index].in_use = 1;
            memoryTable[worst_index].block_start = block_start(worst_index);
            memoryTable[worst_index].block_end = block_start(worst_index) + size;
            return 1; // Return success
        } else {
            printf("No suitable block found for size %d\n", size);
            return -1; // Return no space
        }
    } else {
        printf("Requested %d bytes is not power of 2\n", size);
        return -2; // Return invalid request
    }
}

void defragment_memory() {
    /* Swap blocks around to prevent empty blocks between in-use blocks. */

    int write_index = 0; // Index where the next in-use block will be moved.

    // Iterate through the memoryTable to rearrange blocks.
    for (int read_index = 0; read_index < MAX_BLOCKS; read_index++) {
        if (memoryTable[read_index].in_use == 1) { // Found an in-use block
            if (read_index != write_index) {
                // Move the block to the current write_index
                memoryTable[write_index] = memoryTable[read_index];

                // Update block_start and block_end to reflect the new position
                uintptr_t new_start = block_start(write_index);
                uintptr_t size = memoryTable[write_index].block_end - memoryTable[write_index].block_start;
                memoryTable[write_index].block_start = new_start;
                memoryTable[write_index].block_end = new_start + size;

                // Clear the old block
                memoryTable[read_index].in_use = 0;
                memoryTable[read_index].id = 0;
                memoryTable[read_index].block_start = NULL;
                memoryTable[read_index].block_end = NULL;
            }

            write_index++; // Move to the next available write position
        }
    }

    printf("Memory defragmentation completed.\n");
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

void *run_mms(){
	printf("MMS Running...\n");
	initialize_memory();

	while (1) { //FOREVER LOOP
        sem_wait(&full);             // Wait if there are no items in the buffer
        pthread_mutex_lock(&mutx);  // Get buffer access

        BufferItem item = buffer[buff_index];     // Remove an item from the buffer
        printf("MMS taken from buffer: ID %d, Size %d\n", item.id, item.size);

        // Defrag
        DEFRAG_ENABLE ? defragment_memory() : 1;

        // Try to fit block.
		switch(FIT_MODE){
			case 1:
				*item.result = first_fit(item.size, item.id); // send result back to producer.
				break;
			case 2:
				*item.result = best_fit(item.size, item.id); // send result back to producer.
				break;
			case 3:
				*item.result = worst_fit(item.size, item.id); // send result back to producer.
				break;
			default:
				perror("FIT_MODE not set properly in MMS.h: Needs to be int 1, 2, or 3.\n");
		}

        pthread_mutex_unlock(&mutx); // Release the mutex
        sem_post(&empty);             // Signal that a slot is now available

		buff_index = (buff_index + 1) % BUFFER_SIZE; // Move to the next slot (circular buffer)

		for(int i = 0; i < MAX_BLOCKS; i++){
			print_data_block(&memoryTable[i]);
		}
    }

	free_memory();
	return NULL;
}

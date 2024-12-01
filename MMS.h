#ifndef MMS_H
#define MMS_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>

// Macros
#define MAX_SIZE (30 * 1024) // 30MB
#define MAX_BLOCKS 10       // Maximum number of blocks in memory.
#define BUFFER_SIZE 10
#define MAX_JOBS 10   // Maximum number of jobs in buffer.
#define BUFFER_SIZE 10

// Buffer item for requests from producers.
typedef struct{
    int size;
    int id;
} BufferItem;

// Data Structure
typedef struct{
    int id;            // Block ID
    int in_use;        // Flag: 1 if the block is in use, 0 otherwise
    void *block_start; // Pointer to the start of the block
    void *block_end;   // Pointer to the end of the block
} DataBlock;

// Global Variables
extern void *start;
extern void *end;
extern DataBlock memoryTable[MAX_BLOCKS];
extern BufferItem buffer[BUFFER_SIZE];
extern sem_t empty;       // Tracks available slots in the buffer
extern sem_t full;        // Tracks filled slots in the buffer
extern pthread_mutex_t mutx; // Ensures mutual exclusion

// Function Prototypes
void print_data_block(DataBlock *block);
void initialize_memory();
void *block_start(int index);
void *block_max_end(int index);
uintptr_t available_space(int index);
int *first_fit(int size, int id);
void free_block(int id);
void free_memory();
void *run_mms();

#endif

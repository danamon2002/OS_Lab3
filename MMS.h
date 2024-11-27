#ifndef MMS_H
#define MMS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

// Macros
#define MAX_SIZE (30 * 1024) // 30MB
#define MAX_BLOCKS 10       // Maximum number of blocks in memory.

// Data Structure
struct DataBlock {
    int id;            // Block ID
    int in_use;        // Flag: 1 if the block is in use, 0 otherwise
    void *block_start; // Pointer to the start of the block
    void *block_end;   // Pointer to the end of the block
};

// Global Variables
extern void *start;
extern void *end;
extern struct DataBlock memoryTable[MAX_BLOCKS];

// Function Prototypes
void print_data_block(struct DataBlock *block);
void initialize_memory();
void *block_start(int index);
void *block_max_end(int index);
uintptr_t available_space(int index);
int *first_fit(int size, int id);
void free_block(int id);
void free_memory();
void test_sequence();
void run_mms();

#endif

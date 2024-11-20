#ifndef MMS_H
#define MMS_H

#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>

// Constants
#define MAX_SIZE 12 * 1024 // 12 KB memory
#define NUM_BLOCKS 30      // Number of partitions

// Memory block structure
typedef struct MemoryBlock {
    void *start;
    size_t size;
    bool is_free;
    struct MemoryBlock *next;
} MemoryBlock;

// Request structure
typedef struct MemoryRequest {
    size_t size;    // Requested memory size
    pthread_t tid;  // Thread ID
} MemoryRequest;

// Function prototypes
void initialize_memory();
void *memory_malloc(size_t size);
void memory_free(void *ptr);
void process_requests();
void enqueue_request(MemoryRequest request);
bool dequeue_request(MemoryRequest *request);

#endif

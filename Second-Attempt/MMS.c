#include "MMS.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

// Global variables
static void *memory_pool;
static MemoryBlock *block_list;
static pthread_mutex_t memory_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t queue_cond = PTHREAD_COND_INITIALIZER;

// Initialize memory pool
void initialize_memory() {
    memory_pool = malloc(MAX_SIZE);
    if (!memory_pool) {
        fprintf(stderr, "Failed to allocate memory pool.\n");
        exit(1);
    }

    // Create initial block list
    block_list = malloc(sizeof(MemoryBlock));
    block_list->start = memory_pool;
    block_list->size = MAX_SIZE;
    block_list->is_free = true;
    block_list->next = NULL;
}

// Memory allocation function (First-fit example)
void *memory_malloc(size_t size) {
    pthread_mutex_lock(&memory_mutex);

    MemoryBlock *current = block_list;
    while (current) {
        if (current->is_free && current->size >= size) {
            current->is_free = false;
            pthread_mutex_unlock(&memory_mutex);
            return current->start;
        }
        current = current->next;
    }

    pthread_mutex_unlock(&memory_mutex);
    return NULL; // No suitable block found
}

// Free memory function
void memory_free(void *ptr) {
    pthread_mutex_lock(&memory_mutex);

    MemoryBlock *current = block_list;
    while (current) {
        if (current->start == ptr) {
            current->is_free = true;
            break;
        }
        current = current->next;
    }

    pthread_mutex_unlock(&memory_mutex);
}

// Request processing (to be run by MMS thread)
void process_requests() {
    MemoryRequest request;
    while (true) {
        pthread_mutex_lock(&queue_mutex);
        while (!dequeue_request(&request)) {
            pthread_cond_wait(&queue_cond, &queue_mutex);
        }
        pthread_mutex_unlock(&queue_mutex);

        // Process request
        void *allocated = memory_malloc(request.size);
        if (allocated) {
            printf("Thread %lu allocated %zu bytes.\n", request.tid, request.size);
        } else {
            printf("Thread %lu failed to allocate memory.\n", request.tid);
        }
    }
}

// Queue operations (implementation left to the user)
void enqueue_request(MemoryRequest request) {
    // Add request to queue (to be implemented)
}
bool dequeue_request(MemoryRequest *request) {
    // Remove request from queue (to be implemented)
    return false;
}

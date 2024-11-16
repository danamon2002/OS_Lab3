#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

// Constants
#define MAX_SIZE (12 * 1024) // Total memory size in bytes (12 KB)
#define MAX_REQUESTS 10      // Maximum number of requests in the buffer

// Struct to represent a memory block
typedef struct MemoryBlock {
    size_t size;
    bool is_free;
    void *start;
    struct MemoryBlock *next;
} MemoryBlock;

// Struct to represent a memory request
typedef struct MemoryRequest {
    int thread_id;
    size_t size;
    void **result; // Pointer to store the allocated memory address
} MemoryRequest;

// Global Variables
MemoryBlock *memory_head = NULL; // Start of memory linked list
MemoryRequest request_buffer[MAX_REQUESTS];
int request_count = 0; // Number of requests in the buffer

// Synchronization primitives
pthread_mutex_t memory_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t request_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t request_cond = PTHREAD_COND_INITIALIZER;

// Function prototypes
void initialize_memory();
void *memory_malloc(size_t size);
void memory_free(void *ptr);
void *memory_management_thread(void *arg);
void *user_thread(void *arg);
void *first_fit(size_t size);
void *best_fit(size_t size);
void *worst_fit(size_t size);
void enqueue_request(MemoryRequest request);
MemoryRequest dequeue_request();

// Main function
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number of user threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int num_users = atoi(argv[1]);
    if (num_users <= 0) {
        fprintf(stderr, "Please specify a positive number of user threads.\n");
        exit(EXIT_FAILURE);
    }

    pthread_t *threads = malloc((num_users + 1) * sizeof(pthread_t));
    if (!threads) {
        perror("Failed to allocate memory for threads");
        exit(EXIT_FAILURE);
    }

    initialize_memory();

    // Create memory management thread
    if (pthread_create(&threads[0], NULL, memory_management_thread, NULL) != 0) {
        perror("Failed to create memory management thread");
        free(threads);
        exit(EXIT_FAILURE);
    }

    // Create user threads
    for (int i = 1; i <= num_users; i++) {
        int *thread_id = malloc(sizeof(int));
        if (!thread_id) {
            perror("Failed to allocate memory for thread ID");
            exit(EXIT_FAILURE);
        }
        *thread_id = i;
        if (pthread_create(&threads[i], NULL, user_thread, thread_id) != 0) {
            perror("Failed to create user thread");
            free(thread_id);
            exit(EXIT_FAILURE);
        }
    }

    // Join all threads
    for (int i = 0; i <= num_users; i++) {
        pthread_join(threads[i], NULL);
    }

    // Clean up
    free(threads);
    free(memory_head);
    return 0;
}

// Initialize memory
void initialize_memory() {
    memory_head = malloc(sizeof(MemoryBlock));
    if (!memory_head) {
        perror("Failed to initialize memory");
        exit(EXIT_FAILURE);
    }
    memory_head->size = MAX_SIZE;
    memory_head->is_free = true;
    memory_head->start = malloc(MAX_SIZE);
    memory_head->next = NULL;

    if (!memory_head->start) {
        perror("Failed to allocate memory pool");
        exit(EXIT_FAILURE);
    }
}

// Memory allocation based on First-fit
void *first_fit(size_t size) {
    MemoryBlock *current = memory_head;
    while (current) {
        if (current->is_free && current->size >= size) {
            current->is_free = false;
            return current->start;
        }
        current = current->next;
    }
    return NULL; // No suitable block found
}

// General memory allocation function
void *memory_malloc(size_t size) {
    return first_fit(size); // Can switch to best_fit or worst_fit
}

// Free memory
void memory_free(void *ptr) {
    MemoryBlock *current = memory_head;
    while (current) {
        if (current->start == ptr) {
            current->is_free = true;
            break;
        }
        current = current->next;
    }
}

// Enqueue a memory request
void enqueue_request(MemoryRequest request) {
    pthread_mutex_lock(&request_lock);
    while (request_count == MAX_REQUESTS) {
        pthread_cond_wait(&request_cond, &request_lock); // Wait if the buffer is full
    }
    request_buffer[request_count++] = request;
    pthread_cond_signal(&request_cond);
    pthread_mutex_unlock(&request_lock);
}

// Dequeue a memory request
MemoryRequest dequeue_request() {
    pthread_mutex_lock(&request_lock);
    while (request_count == 0) {
        pthread_cond_wait(&request_cond, &request_lock); // Wait if the buffer is empty
    }
    MemoryRequest request = request_buffer[--request_count];
    pthread_cond_signal(&request_cond);
    pthread_mutex_unlock(&request_lock);
    return request;
}

// Memory management thread
void *memory_management_thread(void *arg) {
    while (true) {
        MemoryRequest request = dequeue_request();
        pthread_mutex_lock(&memory_lock);
        *request.result = memory_malloc(request.size);
        pthread_mutex_unlock(&memory_lock);
    }
    return NULL;
}

// User thread
void *user_thread(void *arg) {
    int thread_id = *(int *)arg;
    free(arg);

    size_t request_size = (rand() % (MAX_SIZE / 4)) + 1; // Random size up to 1/4 of MAX_SIZE
    void *allocated_memory = NULL;

    printf("Thread #%d: Requesting %zu bytes.\n", thread_id, request_size);
    MemoryRequest request = {thread_id, request_size, &allocated_memory};
    enqueue_request(request);

    sleep(1); // Simulate work
    if (allocated_memory) {
        printf("Thread #%d: Memory allocated.\n", thread_id);
        memory_free(allocated_memory);
        printf("Thread #%d: Memory freed.\n", thread_id);
    } else {
        printf("Thread #%d: Memory allocation failed.\n", thread_id);
    }

    return NULL;
}

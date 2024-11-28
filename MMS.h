#ifndef MMS_H
#define MMS_H

#include <pthread.h>
#include <semaphore.h>

// Macros
#define BUFFER_SIZE 6
#define MAX_SIZE (10 * 1024) // 10MB

// Typedefs
typedef int buffer_item;

// Shared Buffers
extern buffer_item userBuffer[BUFFER_SIZE];
extern buffer_item bp_buffer[BUFFER_SIZE];

// Shared Global Variables
extern int index_counter;
extern int userNum;
extern int jobsDone;
extern long* mem;
extern long* part[5];

// Synchronization Primitives
extern sem_t bin_sem;
extern pthread_mutex_t mutx;

// Function Prototypes
void* mms_thread(void* arg);
void* user_thread(void* arg);
long* memory_malloc(int size, long bl[5]);

#endif

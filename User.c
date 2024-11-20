#include "MMS.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *user_thread(void *arg) {
    size_t size = (size_t)arg; // Requested size

    // Make a memory request
    MemoryRequest request = { .size = size, .tid = pthread_self() };
    enqueue_request(request);

    // Sleep for a random time
    printf("I am thread # %lu going to sleep.\n", pthread_self());
    sleep(rand() % 3 + 1);

    // Free memory after waking up
    memory_free(request.size);
    printf("I am thread #%lu waking up.\n", pthread_self());

    return NULL;
}

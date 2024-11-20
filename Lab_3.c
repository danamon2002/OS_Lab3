#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
//User defined headers for user and mms thread functions.
#include "MMS.h"
#include "User.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_threads>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    pthread_t threads[num_threads + 1]; // +1 for MMS thread

    // Initialize MMS
    initialize_memory();

    // Start MMS thread
    pthread_create(&threads[0], NULL, (void *(*)(void *))process_requests, NULL);

    // Start user threads
    for (int i = 1; i <= num_threads; i++) {
        pthread_create(&threads[i], NULL, user_thread, (void *)(rand() % MAX_SIZE));
    }

    // Wait for threads to finish
    for (int i = 0; i <= num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Cleanup
    free(memory_pool);
    return 0;
}

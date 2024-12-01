/*	Author: Dana Maloney
	For Operating Systems FA24 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include<signal.h>
#include "MMS.h"

#define BUFFER_SIZE 10
BufferItem buffer[BUFFER_SIZE]; // buffer for memory requests.
int in = 0; // Index for the producer to insert items

// Define sync tools (sem, mux)
sem_t empty;       // Tracks available slots in the buffer
sem_t full;        // Tracks filled slots in the buffer
pthread_mutex_t mutx; // Ensures mutual exclusion

/* Function defs below  vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */

void *user_thread(){
    /* User thread:
     * Should place requests for memory into buffer.
     * Then, go to sleep.
     * Then, wake up and free memory.*/
	int id = pthread_self();
    srand(id);

    // every user should make 3 requests total.
    for(int requests = 0; requests < 3; requests++){
        int sleepTime = rand() % 10 + 1; // add 1 so there's always *some* sleep time.
        int power = rand() % 14;
        int size = 2;
        int result = 0; // pass address to mms through buffer.
        for(int i = 0; i < power; i++)
            size *= 2;

        // Make request item for buffer.
        BufferItem request_block;
        request_block.size = size;
        request_block.id = id;
        request_block.result = &result;
        //secure buffer for putting item in.
        //printf("USER ID# %d waiting for empty semaphore...\n", id);
        sem_wait(&empty);
        //printf("USER ID# %d waiting for mutex to unlock...\n", id);
        while( pthread_mutex_lock(&mutx)){ // loop until mutex lock obtained
            //do nothing.
        }
        // make thread put request in buffer.
        printf("USER ID# %d writing request.\n", id);
        buffer[in] = request_block;
        in = (in + 1) % BUFFER_SIZE;
        //release mutex.
        if (pthread_mutex_unlock(&mutx)){
            perror("Failed to release mutex from a consumer");
        }
        sem_post(&full); // post new item available.
        while (!result){ // wait for result to update.
            //do nothing.
        }
        //printf("Result was %d\n", result);
        switch(result){
            case 0: // success
                break;
//             case -1: // no space, try request again.
//                 goto buffer_full_retry;
//                 break;
            case -2: // not a power of 2.
                printf("Request wasn't a power of 2.\n");
                goto skip_block;
                break;
            default:
                //With more time, I'd like to make this handle receiving the address of the memory block.
                break;
        }
        // go eepy.
        printf("I am thread #%d going to sleep.\n", id);
        sleep(sleepTime);
        //free up memory.
        free_block(id);
        skip_block:
        printf("I am thread #%d Waking Up.\n");
    }
	return NULL;
}

void run_threads(int n) {
	/* Function to create n+1 threads. */

   	pthread_t *threads = malloc((n + 1) * sizeof(pthread_t)); // Array to store thread IDs
	if (threads == NULL) {
	        perror("Failed to allocate memory for threads");
	        exit(EXIT_FAILURE);
    }

    // Create Memory Management Thread.
    int *arg = malloc(sizeof(int)); // Allocate memory for the thread argumentfg
    if (arg == NULL) {
    	perror("Failed to allocate memory for MMS argument");
    	exit(EXIT_FAILURE);
    }

    *arg = n; // memory manager will run on last thread.

    if( pthread_create(&threads[n], NULL, run_mms, arg) != 0) {
    	perror("Failed to create MMS thread.");
    	free(arg);
    }

    // Create user threads.
    for (int i = 0; i < n; i++) {
        
        if (pthread_create(&threads[i], NULL, user_thread, NULL) != 0) {
            perror("Failed to create User thread.");
            free(arg);
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i <= n; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads); // Free the thread array once everything's done.
    printf("All threads closed. Goodbye.\n");
}

int main(int argc, char *argv[]){
	if (argc < 2){ // Check if arg was given
		printf("Argument required! (Integer)\n"); // Tell user to enter arg.
		return 1;
	} else {
		int arg_1_int = atoi(argv[1]);
		printf("Argument was %d\n", arg_1_int);
        //Init semaphores
        sem_init(&empty, 0, BUFFER_SIZE); // `BUFFER_SIZE` slots are initially empty
        sem_init(&full, 0, 0);            // Initially, no items in the buffer
        //Init mutex
        if(pthread_mutex_init(&mutx, NULL)) {
            puts("Error mutex initialization");
        }

		// call function to make n+1 threads.
		run_threads(arg_1_int);

        //cleanup.
        sem_destroy(&empty);
        sem_destroy(&full);
        pthread_mutex_destroy(&mutx);	// destroy mutex
        free_memory();

	}

	return 0;

}

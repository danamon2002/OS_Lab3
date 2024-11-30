/*	Author: Dana Maloney
	For Operating Systems FA24 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include<signal.h>
#include "MMS.h"

// Define buffer
typedef struct {
    int size;
    int id;
} BufferItem;

#define BUFFER_SIZE 10
BufferItem buffer[BUFFER_SIZE]; // buffer for memory requests.
int in = 0; // Index for the producer to insert items

// Define sync tools (sem, mux)
sem_t empty;       // Tracks available slots in the buffer
sem_t full;        // Tracks filled slots in the buffer
pthread_mutex_t mutx; // Ensures mutual exclusion

/* Function defs below  vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */

void *user_thread(void *arg){
    /* User thread:
     * Should place requests for memory into buffer.
     * Then, go to sleep.
     * Then, wake up and free memory.*/
	int num = *(int *)arg; // Cast and dereference the argument
	int id = pthread_self();
    srand(id);
    int sleepTime = rand() % 10 + 1; // add 1 so there's always *some* sleep time.
    int power = rand() % 14;
    int size = 2;
    for(int i = 0; i < power; i++)
        size *= 2;

    sleep(1); // let the MMS thread start.
    // Make request item for buffer.
    BufferItem request_block;
    request_block.size = size;
    request_block.id = num; // TODO change to thread ID?
    //secure buffer for puttinng item in.
    sem_wait(&empty);
    while( pthread_mutex_lock(&mutx)){ // TODO verify that this works: Should loop until mutex lock obtained
        //do nothing.
    }
    // make thread put request in buffer.
    buffer[in] = request_block;
    in = (in + 1) % BUFFER_SIZE;
    //release mutex.
    if (pthread_mutex_unlock(&mutx)){
        perror("Failed to release mutex from a consumer");
    }
    // go eepy.
	printf("I am thread #%d going to sleep.\n", id);
    sleep(sleepTime);
    //free up memory.
    free_block(id);
	free(arg); // Free the dynamically allocated memory
    printf("I am thread #%d Waking Up.\n");
	return NULL;
}

void run_threads(int n) {
	/* Function to create n+1 threads. */

   	pthread_t *threads = malloc((n + 1) * sizeof(pthread_t)); // Array to store thread IDs
	if (threads == NULL) {
	        perror("Failed to allocate memory for threads");
	        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
    // Create user threads.
        int *arg = malloc(sizeof(int)); // Allocate memory for the thread argument
        if (arg == NULL) {
            perror("Failed to allocate memory for UThread argument");
            exit(EXIT_FAILURE);
        }
        
        *arg = i; // Assign the thread index to the argument
        
        if (pthread_create(&threads[i], NULL, user_thread, arg) != 0) {
            perror("Failed to create User thread.");
            free(arg);
            exit(EXIT_FAILURE);
        }
    }
    
    // Create Memory Management Thread that uses MMS fxn instead of user fxn.
    int *arg = malloc(sizeof(int)); // Allocate memory for the thread argument
    if (arg == NULL) {
    	perror("Failed to allocate memory for MMS argument");
    	exit(EXIT_FAILURE);
    }
    
    *arg = n; // memory manager will run on last thread.
    
    if( pthread_create(&threads[n], NULL, run_mms, arg) != 0) {
    	perror("Failed to create MMS thread.");
    	free(arg);
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
        //Init mutex
        if(pthread_mutex_init(&mutx, NULL)) {
            puts("Error mutex initialization");
        }

        // Lock/unlock functions:
        // pthread_mutex_lock(&mutx);	// mutex lock before critical section
        // pthread_mutex_unlock(&mutx);	// mutex unlock


		// call function to make n+1 threads.
		run_threads(arg_1_int);

        //breakdown mutex.
        pthread_mutex_destroy(&mutx);	// destroy mutex

        //print final state: If all threads are done, there should be NO memory blocks in use!
        printf("FINAL STATE:\n");
		for(int i = 0; i < MAX_BLOCKS; i++){
            print_data_block(&memoryTable[i]);
        }
	}
}

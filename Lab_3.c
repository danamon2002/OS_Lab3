/*	Author: Dana Maloney
	For Operating Systems FA24 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include<signal.h>
#include "MMS.h"

/* Function defs below  vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */

void *user_thread(void *arg){
	int num = *(int *)arg; // Cast and dereference the argument
	int id = pthread_self();
    srand(id);
    int sleepTime = rand() % 10 + 1; // add 1 so there's always *some* sleep time.
    int power = rand() % 14;
    int size = 2;
    for(int i = 0; i < power; i++)
        size *= 2;

    sleep(1); // let the MMS thread start. TODO make thread put request in buffer.
    first_fit(size, id);
	printf("I am thread #%d going to sleep.\n", id);
    sleep(sleepTime);
    free_block(id);
	free(arg); // Free the dynamically allocated memory
    printf("I am thread #%d Waking Up.\n");
	return NULL;
}

void create_threads(int n) {
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
		// call function to make n+1 threads.
		create_threads(arg_1_int);
		for(int i = 0; i < MAX_BLOCKS; i++){
            print_data_block(&memoryTable[i]);
        }
	}
}

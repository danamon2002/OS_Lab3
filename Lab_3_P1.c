#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define int MAX_SIZE (30 * 1024); // maximum memory allocation, in bytes

/*	Author: Dana Maloney
	For Operating Systems FA24 */

void *dummy_function(void *arg){
	int num = *(int *)arg; // Cast and dereference the argument
	printf("Thread %d is running.\n", num);
	free(arg); // Free the dynamically allocated memory
	return NULL;
}

void *memory_management(void *arg){
	int num = *(int *)arg; // Cast and dereference the argument
	printf("Memory manager running on thread %d.\n", num);
	
	// function body: Allocate memory (in kilobytes) based on max_size arg.
	void *sys_mem = malloc(MAX_SIZE);
	printf("Allocated %d bytes in memory.\n", num_kb * 1024);
	if (!sys_mem){
		perror("Memory Allocation failed.");
	}
	
	free(arg); // Free the dynamically allocated memory
	free(sys_mem); // free fixed allocated memory.
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
        
        if (pthread_create(&threads[i], NULL, dummy_function, arg) != 0) {
            perror("Failed to create User thread.");
            free(arg);
            exit(EXIT_FAILURE);
        }
    }
    
    // Create Memory Management Thread that uses MMS fxn instead of user fxn.
    //if (int *malloc(sizeof(int) == NULL){
    int *arg = malloc(sizeof(int)); // Allocate memory for the thread argument
    if (arg == NULL) {
    	perror("Failed to allocate memory for MMS argument");
    	exit(EXIT_FAILURE);
    }
    
    *arg = n; // memory manager will run on last thread.
    
    if( pthread_create(&threads[n], NULL, memory_management, arg) != 0) {
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
		return arg_1_int;
	}
}

<<<<<<< Updated upstream:First-Attempt/Lab_3.c
/*	Author: Dana Maloney
	For Operating Systems FA24 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
=======
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
>>>>>>> Stashed changes:Lab_3.c
#include "MMS.h"

// Define and initialize shared variables
buffer_item userBuffer[BUFFER_SIZE] = {0};
buffer_item bp_buffer[BUFFER_SIZE] = {0};
int index_counter = 0;
int userNum = 0;
int jobsDone = 0;
long* mem = NULL;
long* part[5] = {NULL};

sem_t bin_sem;
pthread_mutex_t mutx;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("1 argument required: Enter integer for number of user threads.\n");
        return 1;
    } else {
        userNum = atoi(argv[1]);
    }

    if (userNum < 1) {
        printf("Error: Argument was %d, but must be at least 1.\n", userNum);
        exit(1);
    }

    mem = (long*)malloc(MAX_SIZE);
    if (mem == NULL) {
        printf("Memory not allocated.\n");
        exit(0);
    }
    printf("Memory successfully allocated using malloc.\n");


    long* part[] = {mem, mem + 3000, mem + 3250, mem + 7250, mem + 8000, mem + 10000};

    for(int i = 0; i < 5; i++){
        printf("Partition %d: Size = %lu\n", i, (part[i] - part[i + 1]));
    }

    pthread_t mms;
    pthread_t users[userNum];
    void* thread_result;

    int state1 = pthread_mutex_init(&mutx, NULL);
    int state2 = sem_init(&bin_sem, 0, 0);
    if (state1 || state2 != 0) {
        puts("Error mutex & semaphore initialization!!!");
    }

    pthread_create(&mms, NULL, mms_thread, &part);

    for (int i = 0; i < userNum; i++) {
        pthread_create(&users[i], NULL, user_thread, &part);
    }

    pthread_join(mms, &thread_result);

    for (int i = 0; i < userNum; i++) {
        pthread_join(users[i], &thread_result);
    }

    printf("\nAll jobs complete & threads joined.\n");

    // get rid of mutex & sem
    sem_destroy(&bin_sem);
    pthread_mutex_destroy(&mutx);

    free(mem);
    printf("All allocated memory freed. Goodbye..\n");

    return 0;
}

<<<<<<< Updated upstream:First-Attempt/Lab_3.c
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
=======
void *user_thread(void *arg) {
  int j;

  int id = pthread_self();
  printf("\nCreating Thread: User #%d\n", (unsigned int)id);

  // Giving User a job of some random size
  int upper_bound = 4000; // Upper bound of job size
  int lower_bound = 1;    // Lower bound of job size
  int job = (99999999999999 - id) % (upper_bound - lower_bound + 1)
  + lower_bound;
>>>>>>> Stashed changes:Lab_3.c

  for(j=0;j<1;)		// Loops until their 1 job is done.
  {

  if (index_counter < BUFFER_SIZE){
      // Controlling when user affects the interface buffer.
      sem_wait(&bin_sem);
      pthread_mutex_lock(&mutx);

      long* blocks = (long*) arg;
      int requestedBlockSize;
      long* ret;
      ret = memory_malloc(job, blocks);

      if(ret == NULL){
      	printf("ERROR. Job too big for memory blocks.\n");
      	sem_post(&bin_sem);
      	pthread_mutex_unlock(&mutx);
      	j++;
        jobsDone++;
      }
      else{

      	for(int h = 0; h < 5; h++){

      		if (ret == (void*)blocks[h]){
      			requestedBlockSize = blocks[h+1] - blocks[h];
      			requestedBlockSize = requestedBlockSize/8;
      		}
      	}

      	userBuffer[index_counter] = requestedBlockSize;
      	// Current position of index-counter saved,
      	int position = index_counter;

      	// index_counter is incremented for next user
      	// and mutex and semaphore are unlocked and increased.
      	index_counter++;
      	sem_post(&bin_sem);
      	pthread_mutex_unlock(&mutx);


      	//looping until MMS gives the user the requested memory block
      	while(bp_buffer[position] == 0){
      	}

      	// User is allocated memory and sleeps
      	// The sleeping mimcs the time it takes for a job to finish.
      	// once user wakes the memory is removed from the bp_buffer
      	printf("I am thread %d, going to sleep\n", id);

      	// Make user sleep for a random amount of time
//     	int upper_bound = 3; // Set the upper bound for sleepValue
//     	int lower_bound = 1; // Set the lower bound for sleepValue
//     	// The user id is subtracted from a large numer to mimic randomness.
//       	int sleepValue = (99999999999999 - id) % (upper_bound - lower_bound + 1)
        srand(time(NULL));
        int max_sleep = 5;
        int sleepValue = rand() % (max_sleep + 1);
        sleep(sleepValue);

      	// User wakes up, which means their job is finished.
      	printf("I am thread %d, waking up\n", id);

      	//User changing buffer values after waking.
      	bp_buffer[position] = 0;
      	userBuffer[position] = 0;

      	// Job is done, so break the 'for' loop and increment jobsDone
      	j++;
        jobsDone++;


      }
     }
    }
}

// First-fit
/*  Value returned is a pointer to start of the first
    memory block found that fits the job. */
long* memory_malloc(int size, long bl[5]){
   // Function will return NULL if no matching partition is found.
   for (int i = 0; i < 5; i++){
   	if((bl[i+1] - bl[i])/8 >= size){
   		return (long*)bl[i];
   	}
   }
   return NULL;
}


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define TRUE 1
typedef int buffer_item;
#define BUFFER_SIZE 8

buffer_item START_NUMBER;

int insert_item(buffer_item item);

int remove_item(buffer_item *item);


buffer_item buffer[BUFFER_SIZE];

pthread_mutex_t mutex;
sem_t empty;
sem_t full;

int insertPointer = 0, removePointer = 0;

void *producer(void *param);
void *consumer(void *param);

int insert_item(buffer_item item)
{
    /* Implementation of the insert_item() function */

	return 0;
}


int remove_item(buffer_item *item)
{
	/* Implementation of the remove_item function */

	return 0;
}


int main(int argc, char *argv[])
{
/* 1. Get command line arguments argv[1],argv[2],argv[3],argv[4] */
/* 2. Initialize buffer */
/* 3. Create producer thread(s) */
/* 4. Create consumer thread(s) */
/* 5. Sleep */
/* 6. Exit */

	int sleepTime, producerThreads, consumerThreads;
	int i, j;

	if(argc != 5)
	{
		fprintf(stderr, "Useage: <sleep time> <producer threads> <consumer threads> <start number>\n");
		return -1;
	}

	sleepTime = atoi(argv[1]);
	producerThreads = atoi(argv[2]);
	consumerThreads = atoi(argv[3]);
	START_NUMBER = atoi(argv[4]);

	/* Initialize the synchronization tools */

	/* Create the producer and consumer threads */

	/* Sleep for user specified time */
	sleep(sleepTime);

	return 0;
}

void *producer(void *param)
{
	/* Implementation of the producer thread -- refer to Figure 5.26 on page 256 */

}

void *consumer(void *param)
{
	/* Implementation of the consumer thread -- refer to Figure 5.26 on page 256 */
}

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define TRUE 1
typedef int buffer_item;
#define BUFFER_SIZE 8

int USE_MICROSECONDS = 1;

char FIRST_INITIAL = 'm';
char *LAST_NAME = "anderson";

buffer_item START_NUMBER;

typedef struct
{
	int index;
} parameter;

buffer_item produce_value;

buffer_item buffer[BUFFER_SIZE];

pthread_mutex_t mutex;
pthread_mutex_t value_mutex;
sem_t empty;
sem_t full;

int insertPointer = 0, removePointer = 0;

void *producer(void *param);
void *consumer(void *param);

int insert_item(buffer_item item, int index);

int remove_item(buffer_item *item, int index);

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

	if (argc != 5)
	{
		printf("Useage: <sleep time> <producer threads> <consumer threads> <start number>\n");
		return -1;
	}

	sleepTime = atoi(argv[1]);
	producerThreads = atoi(argv[2]);
	consumerThreads = atoi(argv[3]);
	START_NUMBER = atoi(argv[4]);

	/* Initialize the synchronization tools */
	if (pthread_mutex_init(&mutex, NULL) != 0)
	{
		printf("mutex init has failed\n");
		return 1;
	}
	if (pthread_mutex_init(&value_mutex, NULL) != 0)
	{
		printf("mutex init has failed\n");
		return 1;
	}
	sem_init(&empty, 1, BUFFER_SIZE);
	sem_init(&full, 1, 0);

	produce_value = START_NUMBER;

	/* Create the producer and consumer threads */
	pthread_t producers[producerThreads];
	for (int i = 0; i < producerThreads; i++)
	{
		parameter *data = (parameter *)malloc(sizeof(parameter));
		data->index = i;
		pthread_create(&producers[i], NULL, producer, data);
	}

	pthread_t consumers[consumerThreads];
	for (int i = 0; i < consumerThreads; i++)
	{
		parameter *data = (parameter *)malloc(sizeof(parameter));
		data->index = i;
		pthread_create(&consumers[i], NULL, consumer, data);
	}

	if (USE_MICROSECONDS == 0)
	{
		/* Sleep for user specified number of seconds */
		sleep(sleepTime * 1000000);
	}
	else
	{
		/* Sleep for user specified number of milliseconds */
		usleep(sleepTime);
	}

	sem_destroy(&empty);
	sem_destroy(&full);

	if (pthread_mutex_lock(&mutex) != 0)
	{
		return -1;
	}

	printf("DONE\n");

	return 0;
}

void *producer(void *param)
{
	/* Implementation of the producer thread -- refer to Figure 5.26 on page 256 */

	parameter *data = (parameter *)param;
	int index = data->index;
	free(data);
	buffer_item item;
	while (TRUE)
	{
		/* generate/retrieve item */
		pthread_mutex_lock(&value_mutex);
		item = produce_value++;

		/* insert item */
		insert_item(item, index);
		pthread_mutex_unlock(&value_mutex);
	}
}

void *consumer(void *param)
{
	/* Implementation of the consumer thread -- refer to Figure 5.26 on page 256 */
	parameter *data = (parameter *)param;
	int index = data->index;
	free(data);
	buffer_item item;
	while (TRUE)
	{
		remove_item(&item, index);
		/* process item */
	}
}

int insert_item(buffer_item item, int index)
{
	/*
		insert item into buffer
		return 0 if successful
		return -1 if not successful
	*/

	// if (sem_wait(&empty) != 0)
	// {
	// 	return -1;
	// }
	// if (pthread_mutex_lock(&mutex) != 0)
	// {
	// 	return -1;
	// }
	sem_wait(&empty);
	pthread_mutex_lock(&mutex);

	buffer[insertPointer] = item;
	insertPointer++;
	insertPointer %= BUFFER_SIZE;
	printf("Producer %c%s_P%d produced %d\n",
		   FIRST_INITIAL, LAST_NAME, index, item);
	// if (sem_post(&full) != 0)
	// {
	// 	return -1;
	// }
	// if (pthread_mutex_unlock(&mutex) != 0)
	// {
	// 	return -1;
	// }
	sem_post(&full);
	pthread_mutex_unlock(&mutex);
	return 0;
}

int remove_item(buffer_item *item, int index)
{
	/*
		remove object from buffer
		place it in item
		return 0 if successful
		return -1 if not successful
	*/
	// if (sem_wait(&full) != 0)
	// {
	// 	return -1;
	// }
	// if (pthread_mutex_lock(&mutex) != 0)
	// {
	// 	return -1;
	// }
	sem_wait(&full);
	pthread_mutex_lock(&mutex);
	*item = buffer[removePointer];
	removePointer++;
	removePointer %= BUFFER_SIZE;
	printf("Consumer %c%s_C%d comsumed %d\n",
		   FIRST_INITIAL, LAST_NAME, index, *item);
	// if (sem_post(&empty) != 0)
	// {
	// 	return -1;
	// }
	// if (pthread_mutex_unlock(&mutex) != 0)
	// {
	// 	return -1;
	// }
	sem_post(&empty);
	pthread_mutex_unlock(&mutex);

	return 0;
}

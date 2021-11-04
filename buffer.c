#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define TRUE 1
typedef int buffer_item;
#define BUFFER_SIZE 8

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
pthread_mutex_t print_mutex;
sem_t empty;
sem_t full;

void printBuffer();

void *producer(void *param);
void *consumer(void *param);

int insert_item(buffer_item item);

int remove_item(buffer_item *item);

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

	//printf("top of main\n");

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
	if (pthread_mutex_init(&print_mutex, NULL) != 0)
	{
		printf("mutex init has failed\n");
		return 1;
	}
	sem_init(&empty, 1, BUFFER_SIZE);
	sem_init(&full, 1, 0);

	//printf("inits done\n");

	produce_value = START_NUMBER;

	/* Create the producer and consumer threads */
	//printf("creating producers\n");
	pthread_t producers[producerThreads];
	for (int i = 0; i < producerThreads; i++)
	{
		//printf("creating producer %d\n", i);
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

	/* Sleep for user specified time */
	sleep(sleepTime);
	// struct timespec ts;
	// ts.tv_sec = (sleepTime / 1000);
	// ts.tv_nsec = (sleepTime % 1000);
	// nanosleep(&ts, &ts);

	// printf("done");

	return 0;
}

void *producer(void *param)
{
	/* Implementation of the producer thread -- refer to Figure 5.26 on page 256 */

	parameter *data = (parameter *)param;
	int index = data->index;
	buffer_item item;
	free(data);
	while (TRUE)
	{
		pthread_mutex_lock(&value_mutex);
		item = produce_value++;
		pthread_mutex_unlock(&value_mutex);
		// printf("producer_%d with item %d\n", index, item);
		if (insert_item(item))
		{
			pthread_mutex_lock(&print_mutex);
			printf("error producing item %d\n", item);
			pthread_mutex_unlock(&print_mutex);
		}
		else
		{
			pthread_mutex_lock(&print_mutex);
			printf("Producer %c%s_%d produced %d\n",
				   FIRST_INITIAL, LAST_NAME, index, item);
			pthread_mutex_unlock(&print_mutex);
		}
		// sleep(1);
	}
}

void *consumer(void *param)
{
	/* Implementation of the consumer thread -- refer to Figure 5.26 on page 256 */
	parameter *data = (parameter *)param;
	int index = data->index;
	buffer_item item;
	free(data);

	// printf("consumer_%d\n", index);
	while (TRUE)
	{
		if (remove_item(&item))
		{
			pthread_mutex_lock(&print_mutex);
			printf("error consuming item %d\n", item);
			pthread_mutex_unlock(&print_mutex);
		}
		else
		{
			pthread_mutex_lock(&print_mutex);
			printf("Consumer %c%s_%d comsumed %d\n",
				   FIRST_INITIAL, LAST_NAME, index, item);
			pthread_mutex_unlock(&print_mutex);
		}
		// sleep(1);
	}
}

int insert_item(buffer_item item)
{
	/*
		insert item into buffer
		return 0 if successful
		return -1 if not successful
	*/
	// printf("waiting to insert %d\n", item);
	sem_wait(&empty);
	pthread_mutex_lock(&mutex);
	int *idx;
	sem_getvalue(&full, idx);
	// printf("inserting %d at [%d]\n", item, *idx);
	buffer[*idx] = item;
	// printBuffer();
	sem_post(&full);
	// int *e, *f;
	// sem_getvalue(&empty, e);
	// sem_getvalue(&full, f);
	// printf("%d:%d\n", *e, *f);
	pthread_mutex_unlock(&mutex);
	return 0;
}

int remove_item(buffer_item *item)
{
	/*
		remove object from buffer
		place it in item
		return 0 if successful
		return -1 if not successful
	*/
	// printf("waiting to remove\n");
	sem_wait(&full);
	pthread_mutex_lock(&mutex);
	int *idx;
	sem_getvalue(&full, idx);
	// printf("consuming [%d]\n", *idx);
	*item = buffer[*idx];
	// printBuffer();
	// printf("removing %d from [%d]\n", *item, *idx);
	sem_post(&empty);
	pthread_mutex_unlock(&mutex);

	return 0;
}

void printBuffer()
{
	printf("%d", buffer[0]);
	for (int i = 1; i < BUFFER_SIZE; i++)
	{
		printf(",%d", buffer[i]);
	}
	printf("\n");
}
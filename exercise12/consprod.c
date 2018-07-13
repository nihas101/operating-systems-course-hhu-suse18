#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "queue.h"

typedef struct arguments{
  queue_t *queue;
  int nr_of_items_per_cons;
  int nr_of_items_per_prod;
} argument_t;

sem_t mutex;

pthread_t* create_producer_threads(argument_t *arguments, int nr_of_producers);
pthread_t* create_consumer_threads(argument_t *arguments, int nr_of_consumers);
void *produce(void *arguments);
void *consume(void *arguments);

int main(int argc, char const *argv[]) {
  if(argc < 4){
    perror("Please provide the number of producers, consumers and the number of items to produce.\n");
    perror("Example: consprod 4 2 10000\n");
    exit(EXIT_FAILURE);
  }

  pthread_t *prod_threads;
  pthread_t *cons_threads;

  int nr_of_producers = atoi(argv[1]);
  int nr_of_consumers = atoi(argv[2]);

  queue_t *queue = queue_new(100);

  sem_init(&mutex, 0, 1);

  argument_t *arguments = malloc(sizeof(argument_t));
  if(arguments == NULL){
    perror("main: Failed to allocate enough memory.\n");
    exit(EXIT_FAILURE);
  }
  arguments->queue = queue;
  arguments->nr_of_items_per_cons = atoi(argv[3])/nr_of_consumers;
  arguments->nr_of_items_per_prod = atoi(argv[3])/nr_of_producers;

  cons_threads = (pthread_t *) create_consumer_threads(arguments, nr_of_consumers);
  prod_threads = (pthread_t *) create_producer_threads(arguments, nr_of_producers);

  for(int i=0; i < nr_of_producers; i++){
    if(pthread_join(prod_threads[i], NULL) < 0) {
      perror("An error occured trying to join the producer threads.\n");
      exit(EXIT_FAILURE);
    }
  }

  for(int i=0; i < nr_of_consumers; i++){
    if(pthread_join(cons_threads[i], NULL) < 0) {
      perror("An error occured trying to join the producer threads.\n");
      exit(EXIT_FAILURE);
    }
  }

  sem_destroy(&mutex);
  free(cons_threads);
  free(prod_threads);
  free(arguments);
  queue_delete(queue);

  return 0;
}

pthread_t* create_consumer_threads(argument_t *arguments, int nr_of_consumers){
  pthread_t *threads = malloc(sizeof(pthread_t)*nr_of_consumers);
  if(threads == NULL){
    perror("create_consumer_threads: Failed to allocate enough memory.\n");
    exit(EXIT_FAILURE);
  }

  for(int i=0; i < nr_of_consumers; i++){
    if(pthread_create(&(threads[i]), NULL, consume, arguments) < 0){
      perror("An error occured trying to create the consumer threads.\n");
      exit(EXIT_FAILURE);
    }
  }

  return threads;
}

void *consume(void *args){
  queue_t *queue = ((argument_t *) args)->queue;
  int nr_of_items = ((argument_t *) args)->nr_of_items_per_cons;
  int ret;

  for(int i=0; i < nr_of_items; i++){
    sem_wait(&mutex);
    if(queue_pop_front(queue, &ret) >= 0){
      queue_print(queue);
    }else{
      i--;
    }
    sem_post(&mutex);
  }

  return NULL;
}

pthread_t* create_producer_threads(argument_t *arguments, int nr_of_producers){
    pthread_t *threads = malloc(sizeof(pthread_t)*nr_of_producers);
    if(threads == NULL){
      perror("create_producer_threads: Failed to allocate enough memory.\n");
      exit(EXIT_FAILURE);
    }

    for(int i=0; i < nr_of_producers; i++){
      if(pthread_create(&(threads[i]), NULL, produce, arguments) < 0){
        perror("An error occured trying to create the producer threads.\n");
        exit(EXIT_FAILURE);
      }
    }

    return threads;
}

void *produce(void *args){
  queue_t *queue = ((argument_t *) args)->queue;
  int nr_of_items = ((argument_t *) args)->nr_of_items_per_prod;

  for(int i=0; i < nr_of_items; i++){
    sem_wait(&mutex);
    if(queue_push_back(queue, i) < 0){
      i--;
    }
    sem_post(&mutex);
  }

  return NULL;
}
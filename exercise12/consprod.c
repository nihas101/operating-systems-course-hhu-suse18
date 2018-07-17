#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "queue.h"

#define QUEUE_MAX_SIZE 100

typedef struct arguments{
  queue_t *queue;
  int nr_of_items_per_cons;
  int nr_of_items_per_prod;
} argument_t;

sem_t access_mutex;
sem_t write_mutex, read_mutex;

void join_threads(pthread_t* threads, int nr_of_threads);
pthread_t* create_threads(argument_t *arguments, int nr_of_threads, void *function(void *args));
void *produce(void *arguments);
void *consume(void *arguments);

int main(int argc, char const *argv[]) {
  if(argc < 4){
    perror("Please provide the number of producers, consumers and the number of items to produce.\n");
    perror("Example: consprod 4 2 9000\n");
    exit(EXIT_FAILURE);
  }

  // SETUP
  int nr_of_producers = atoi(argv[1]);
  int nr_of_consumers = atoi(argv[2]);
  int nr_of_items = atoi(argv[3]);

  if(nr_of_items % nr_of_consumers != 0 && nr_of_items % nr_of_producers != 0){
    perror("Please provide a number of items (arg 3) that is divisible by the number of consumers (arg 2) and producers (arg 1).\n");
    exit(EXIT_FAILURE);
  }

  pthread_t *prod_threads;
  pthread_t *cons_threads;
  queue_t *queue = queue_new(QUEUE_MAX_SIZE);

  sem_init(&access_mutex, 0, 1);
  sem_init(&write_mutex, 0, 0);
  sem_init(&read_mutex, 0, QUEUE_MAX_SIZE);

  argument_t *arguments = malloc(sizeof(argument_t));
  if(arguments == NULL){
    perror("main: Failed to allocate enough memory.\n");
    exit(EXIT_FAILURE);
  }

  arguments->queue = queue;
  arguments->nr_of_items_per_cons = nr_of_items/nr_of_consumers;
  arguments->nr_of_items_per_prod = nr_of_items/nr_of_producers;

  // THREAD EXECUTION
  cons_threads = (pthread_t *) create_threads(arguments, nr_of_consumers, consume);
  prod_threads = (pthread_t *) create_threads(arguments, nr_of_producers, produce);

  join_threads(cons_threads, nr_of_consumers);
  join_threads(prod_threads, nr_of_producers);

  // CLEANUP
  sem_destroy(&access_mutex);
  sem_destroy(&write_mutex);
  sem_destroy(&read_mutex);
  free(cons_threads);
  free(prod_threads);
  free(arguments);
  queue_delete(queue);

  return 0;
}

void join_threads(pthread_t* threads, int nr_of_threads){
  for(int i=0; i < nr_of_threads; i++){
    if(pthread_join(threads[i], NULL) < 0) {
      perror("An error occured trying to join the producer threads.\n");
      exit(EXIT_FAILURE);
    }
  }
}

pthread_t* create_threads(argument_t *arguments, int nr_of_threads, void *func(void *args)){
  pthread_t *threads = malloc(sizeof(pthread_t)*nr_of_threads);
  if(threads == NULL){
    perror("create_threads: Failed to allocate enough memory.\n");
    exit(EXIT_FAILURE);
  }

  for(int i=0; i < nr_of_threads; i++){
    if(pthread_create(&(threads[i]), NULL, func, arguments) < 0){
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
    sem_wait(&read_mutex);
    sem_wait(&access_mutex);
    if(queue_pop_front(queue, &ret) >= 0){
      queue_print(queue);
    }else{
      i--;
    }
    sem_post(&write_mutex);
    sem_post(&access_mutex);
  }

  return NULL;
}

void *produce(void *args){
  queue_t *queue = ((argument_t *) args)->queue;
  int nr_of_items = ((argument_t *) args)->nr_of_items_per_prod;

  for(int i=0; i < nr_of_items; i++){
    sem_wait(&write_mutex);
    sem_wait(&access_mutex);
    if(queue_push_back(queue, i) >= 0){
      queue_print(queue);
    }else{
      i--;
    }
    sem_post(&read_mutex);
    sem_post(&access_mutex);
  }

  return NULL;
}

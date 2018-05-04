#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#define ARRAY_SIZE 100

int *create_array(int size);
void free_array(int *array);
void fill_array(int *array, int size);
void print_array(int *array, int size);
int thread_search_max(int *array, int array_size, int number_of_threads);
int min(int a, int b);
void *search_max_thread(void *argc);
void search_max(int *array, int beginning, int end, int *max);
void time_thread_search(int *array, int size, int number_of_threads);

int main() {
  int *array;

  srand(time(NULL));
  array = malloc(sizeof(int)*ARRAY_SIZE);
  fill_array(array, ARRAY_SIZE);

  printf("Array: ");
  print_array(array, ARRAY_SIZE);
  printf("\n");

  time_thread_search(array, ARRAY_SIZE, 1);
  time_thread_search(array, ARRAY_SIZE, 2);
  time_thread_search(array, ARRAY_SIZE, 4);
  time_thread_search(array, ARRAY_SIZE, 8);

  free(array);

  return 0;
}

void time_thread_search(int *array, int size, int number_of_threads){
  clock_t clock_time = clock();
  thread_search_max(array, size, number_of_threads);
  printf("CPU Time: %ld\n\n", clock()-clock_time);
}

void fill_array(int *array, int size){
  for(int i=0; i<size; i++){
    *(array + i) = rand()%100;
  }
}

void print_array(int *array, int size){
  printf("[");
  for(int i=0; i<size; i++){
    printf("%d", *(array + i));
    if(i < size-1) printf(", ");
  }
  printf("]\n");
}

int thread_search_max(int *array, int array_size, int number_of_threads){
  double search_length = (double)array_size/(double)number_of_threads;
  int status = 0;
  int *status_pointer = &status;
  int max;
  pthread_t *threads = malloc(sizeof(pthread_t)*number_of_threads);
  int *maxs = malloc(sizeof(int)*number_of_threads);

  if(threads == NULL){
    printf("An error occured trying to allocate space for the thread-IDs.\n");
    exit(-1);
  }
  if(maxs == NULL){
    printf("An error occured trying to allocate space for the possible maximums.\n");
    exit(-1);
  }

  printf("Searching with %d threads and in lengths of %f.\n", number_of_threads, search_length);

  for(int i=0; i < number_of_threads; i++){
    int *argc = malloc(sizeof(int)*4);
    if(argc == NULL){
      printf("An error occured trying to allocate space for the arguments of search_max_thread.\n");
      exit(-1);
    }
    // Create arguments for the thread call
    argc[0] = (intptr_t) array;
    argc[1] = i*search_length;
    argc[2] = min(argc[1]+search_length, array_size);
    argc[3] = (intptr_t) &maxs[i];

    if(pthread_create(&(threads[i]), NULL, &search_max_thread, argc) != 0){
      printf("An error occured trying to create a thread.\n");
      exit(-1);
    }
  }
  // Wait for all threads to finish
  for(int i=0; i < number_of_threads; i++){
    if(pthread_join(threads[i], (void**)&status_pointer) != 0){
      printf("An error occured in one of the child threads.\n");
      exit(-1);
    }
  }

  printf("Possible Maximums: ");
  print_array(maxs, number_of_threads);

  max = maxs[0];
  search_max(maxs, 0, number_of_threads, &max);
  printf("Maximum found: %d\n", max);

  free(threads);
  free(maxs);

  return max;
}

int min(int a, int b){
  if(a < b){
    return a;
  }else{
    return b;
  }
}

void *search_max_thread(void *argc){
  int *array = ((int*)argc)[0];
  int beginning = (int)((int*)argc)[1];
  int end = (int)((int*)argc)[2];
  int *max = ((int*)argc)[3];
  *max = array[beginning];

  search_max(array, beginning, end, max);

  free(argc);
  return 0;
}

void search_max(int *array, int beginning, int end, int *max){
  for(int i=beginning; i < end; i++){
    if(*max < array[i]){
      *max = array[i];
    }
  }
}

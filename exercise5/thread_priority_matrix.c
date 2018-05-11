#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#include "matrix.h"

typedef struct{
  matrix* matrix_a;
  matrix* matrix_b;
}matrices;

int* time_threads_matrix_multiplication(matrix *matrix_a, matrix *matrix_b, int number_of_threads);
void* time_thread_matrix_multiplication(void* matrices);

int main(int argc, char const *argv[]) {
  if(argc < 6){
    printf("Necessary arguments: <Matrix A Rows> <Matrix A Cols> <Matrix B Rows> <Matrix B Cols> <Nr of Threads>");
    exit(-1);
  }

  int matrix_a_rows = atoi(argv[1]);
  int matrix_a_cols = atoi(argv[2]);
  int matrix_b_rows = atoi(argv[3]);
  int matrix_b_cols = atoi(argv[4]);
  int number_of_threads = atoi(argv[5]);
  matrix *matrix_a, *matrix_b;
  srand(time(NULL));

  if(matrix_a_rows < 1 || matrix_a_cols < 1 || matrix_b_rows < 1 || matrix_b_cols < 1 || number_of_threads < 1){
    printf("The arguments need to be positive integers\n");
    exit(-1);
  }

  matrix_a = create_matrix(matrix_a_rows, matrix_a_cols);
  matrix_b = create_matrix(matrix_b_rows, matrix_b_cols);
  initialize_matrix(matrix_a);
  initialize_matrix(matrix_b);

  time_threads_matrix_multiplication(matrix_a, matrix_b, number_of_threads);

  free_matrix(matrix_a);
  free_matrix(matrix_b);
  return 0;
}

int* time_threads_matrix_multiplication(matrix *matrix_a, matrix *matrix_b, int number_of_threads){
  matrices *matrices_args = malloc(sizeof(matrices));
  pthread_t pthread[number_of_threads];
  void *thread_time[number_of_threads];
  struct sched_param schedule_parameter;
  schedule_parameter.sched_priority = 99;
  int error;

  matrices_args->matrix_a = matrix_a;
  matrices_args->matrix_b = matrix_b;

  for(int i=0; i<number_of_threads; i++){
    error = pthread_create(&pthread[i], NULL, &time_thread_matrix_multiplication, matrices_args);
    if(error){
      printf("An error occured trying to create a thread: %d.\n", error);
      exit(-1);
    }
    if(i==number_of_threads/2){
        pthread_setschedparam(pthread[i], SCHED_FIFO, &schedule_parameter);
    }
  }

  for(int i=0; i<number_of_threads; i++){
    error = pthread_join(pthread[i], &thread_time[i]);
    if(error){
      printf("An error occured trying to join a thread: %d.\n", error);
      exit(-1);
    }
  }

  for(int i=0; i<number_of_threads; i++){
    printf("Thread %d:\t%ld\tms",  i, (long int)thread_time[i]);
    if(i==number_of_threads/2){
        printf("\t[High priority]\n");
    }else{
      printf("\n");
    }
  }

  free(matrices_args);
}

void* time_thread_matrix_multiplication(void *matrices_args){
    struct timespec start, end;
    matrix *matrix_a;
    matrix *matrix_b;
    matrix *matrix_c;
    int error;

    matrix_a = ((matrices*)matrices_args)->matrix_a;
    matrix_b = ((matrices*)matrices_args)->matrix_b;

    error = clock_gettime(CLOCK_MONOTONIC, &start);
    if(error){
      printf("An error occured trying to retrieve the start time: %d.\n", error);
      exit(-1);
    }

    matrix_c = multiply_matrices(matrix_a, matrix_b);

    error = clock_gettime(CLOCK_MONOTONIC, &end);
    if(error){
      printf("An error occured trying to retrieve the start time: %d.\n", error);
      exit(-1);
    }

    free_matrix(matrix_c);
    return (void *)((end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000 / 1000);
}

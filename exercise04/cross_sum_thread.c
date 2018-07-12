#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>


#define READ 0
#define WRITE 1

#define SUCCESS 0
#define ERROR -1

#define BLOCK 0

#define DATA_SIZE 80

void thread_error(int error);
void *calulate_cross_sum(void *data);
void print_cross_sum(int *pipe_ends, char *data, pthread_t thread_id);
int cross_sum(char *integer);
void read_pipe(int *pipe_ends, char *data);

int pipe_ends[2];

int main(int argc, char const *argv[]) {
  char *data = malloc(DATA_SIZE*sizeof(char));
  int pipe_result;
  int thread_result;
  pthread_t thread_id;

  if(argc < 2){
    printf("Please provide a number as an argument.\n");
    free(data);
    exit(-1);
  }

  pipe_result = pipe(pipe_ends);
  if(pipe_result != SUCCESS){
    printf("An error occured trying to create a pipe: %d.\n", pipe_result);
    free(data);
    exit(-1);
  }

  write(pipe_ends[WRITE], argv[1], strlen(argv[1]));

  thread_result = pthread_create(&thread_id, NULL, &calulate_cross_sum, data);
  if (thread_result == SUCCESS) {
    print_cross_sum(pipe_ends, data, thread_id);
  } else {
    thread_error(thread_result);
  }

  free(data);

  return 0;
}

void thread_error(int error){
  printf("An error occured trying to create a thread: %d\n", error);
  exit(-1);
}

void *calulate_cross_sum(void *data){
  read_pipe(pipe_ends, data);
  int sum = cross_sum(data);
  char *sum_string = malloc(DATA_SIZE*sizeof(char));

  sprintf(sum_string, "%d", sum);

  write(pipe_ends[WRITE], sum_string, strlen(sum_string));
  free(sum_string);
  return 0;
}

void print_cross_sum(int *pipe_ends, char *data, pthread_t thread_id){
  int status = 0;
  void *status_pointer = &status;

  pthread_join(thread_id, &status_pointer);

  if(status == 0){
    read_pipe(pipe_ends, data);
    printf("Cross sum: %s\n", data);
  }else printf("An error occured in the child thread: %d\n", status);
}

void read_pipe(int *pipe_ends, char *data){
  int bytes_read;
  bytes_read = read(pipe_ends[READ], data, DATA_SIZE-1);

  if(bytes_read == ERROR){
    printf("An error occured trying to read from the pipe.\n");
    exit(-1);
  }else{
    data[bytes_read] = '\0';
  }
}

int cross_sum(char *integer){
  int length = strlen(integer);
  int sum = 0;

  for(int i=0; i < length; i++){
    sum += *(integer + i) - '0';
  }

  return sum;
}

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1

#define ERROR -1
#define CHILD_PROCESS 0

#define BLOCK 0

#define DATA_SIZE 80

void fork_error(void);
void calulate_cross_sum(int *pipe_ends, char *data);
void print_cross_sum(int *pipe_ends, char *data, pid_t child_process_id);
int cross_sum(char *integer);
void read_pipe(int *pipe_ends, char *data);

int main(int argc, char const *argv[]) {
  char *data = malloc(DATA_SIZE*sizeof(char));
  pid_t child_process_id;
  int pipe_ends[2];

  if(argc < 2){
    printf("Please provide a number as an argument.\n");
    free(data);
    exit(-1);
  }

  pipe(pipe_ends);

  switch (child_process_id = fork()) {
    case ERROR:
      fork_error();
      break;
    case CHILD_PROCESS:
      calulate_cross_sum(pipe_ends,  data);
      break;
    default:
      write(pipe_ends[WRITE], argv[1], strlen(argv[1]));
      print_cross_sum(pipe_ends, data, child_process_id);
      break;
  }

  free(data);

  return 0;
}

void fork_error(){
  printf("An error occured trying to fork.");
  exit(-1);
}

void calulate_cross_sum(int *pipe_ends, char *data){
  read_pipe(pipe_ends, data);
  int sum = cross_sum(data);
  char *sum_string = malloc(DATA_SIZE*sizeof(char));

  sprintf(sum_string, "%d", sum);

  write(pipe_ends[WRITE], sum_string, strlen(sum_string));
  free(sum_string);
}

void print_cross_sum(int *pipe_ends,char *data, pid_t child_process_id){
  int status = 0;

  if(waitpid(child_process_id, &status, BLOCK) != -1){
    read_pipe(pipe_ends, data);
    printf("Cross sum: %s\n", data);
  }else  printf("An error occured in the child process.\n");
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

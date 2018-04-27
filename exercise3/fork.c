#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include "matrix.h"

#define MATRIX_SIZE 500

long long* createMatrix(void);
void multiplyMatricesTimed(long long *matrixA, long long *matrixB, long long *matrixC);
void fork_error(void);
void time_child_process(pid_t child_id);

int main() {
  long long *matrixA = createMatrix();
  long long *matrixB = createMatrix();
  long long *matrixC = createMatrix();

  initializeMatrix(matrixA, MATRIX_SIZE, MATRIX_SIZE);
  initializeMatrix(matrixB, MATRIX_SIZE, MATRIX_SIZE);

  multiplyMatricesTimed(matrixA, matrixB, matrixC);

  free(matrixA);
  free(matrixB);
  free(matrixC);

  return 0;
}

long long* createMatrix(){
  return (long long*) malloc(MATRIX_SIZE*MATRIX_SIZE * sizeof(long long));
}

void multiplyMatricesTimed(long long *matrixA, long long *matrixB, long long *matrixC){
  pid_t child_process_id;

  switch (child_process_id = fork()) {
    case -1:
      fork_error();
      break;
    case 0: // Child process
      multiplyMatrices(matrixA, matrixB, matrixC, MATRIX_SIZE, MATRIX_SIZE, MATRIX_SIZE);
      break;
    default: // Parent process
      time_child_process(child_process_id);
      break;
  }
}

void fork_error(){
  printf("An error occured trying to fork.");
  exit(-1);
}

void time_child_process(pid_t child_id){
  pid_t process_id = 0;
  int status = 0;
  time_t start_time = time(NULL);
  double elapsed_seconds;

  while(process_id != child_id){
    process_id = waitpid(child_id, &status, WNOHANG); // The WNOHANG option returns immediately and doesn't block the process
    elapsed_seconds = difftime(time(NULL), start_time);
    sleep(1);
    printf("Elapsed seconds:\t%d\n", (int)elapsed_seconds);
  }
}

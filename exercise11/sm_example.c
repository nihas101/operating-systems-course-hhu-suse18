#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#define ERROR -1
#define CHILD_PROCESS 0
#define BLOCK 0

#define MAX_CHARS 1024

void fork_error(void);
void child_process(key_t key);
void parent_process(key_t key);

extern int errno;

int main(int argc, char const *argv[]) {
  if(argc < 2){
    perror("Please provide a key (integer) as the first argument.\n");
    exit(EXIT_FAILURE);
  }

  pid_t child_process_id;
  int status = 0;
  key_t key = (key_t) strtol(argv[1], NULL, 10);

  switch (child_process_id = fork()) {
    case ERROR:
      perror("An error occured trying to fork.");
      exit(-1);
      break;
    case CHILD_PROCESS:
      child_process(key);
      break;
    default: // Parent process
      if(waitpid(child_process_id, &status, BLOCK) != -1){
        parent_process(key);
      }else{
        perror("An error occured while wating for the child.\n");
        exit(EXIT_FAILURE);
      }
      break;
  }

  return EXIT_SUCCESS;
}

void child_process(key_t key){
  char* buffer = (char*) malloc(MAX_CHARS);
  if(buffer == NULL){
    perror("Unable to allocate buffer.\n");
    exit(EXIT_FAILURE);
  }

  int shm_id = 0;
  char* shm_data;
  size_t read_characters = (size_t) 0;
  size_t max_chars = (size_t) MAX_CHARS;
  size_t buffer_size = (size_t) MAX_CHARS;

  printf("Type a string to pass > ");
  read_characters = getline(&buffer, &buffer_size, stdin);
  buffer[read_characters-1] = '\0';

  shm_id = shmget(key, max_chars, (IPC_CREAT | 0666));
  if(shm_id == -1){
    printf("Failed to create shared memory: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  shm_data = shmat(shm_id, NULL, 0);
  if(shm_data == (char *)(-1)){
    perror("An error occured trying to attach to the shared memory.\n");
    exit(EXIT_FAILURE);
  }

  strcpy(shm_data, buffer);
  free(buffer);
  shmdt(&shm_data);
}

void parent_process(key_t key){
  int shm_id = 0;
  char* shm_data;
  char* buffer = (char*) malloc(MAX_CHARS);
  size_t max_chars = (size_t) MAX_CHARS;
  shm_id = shmget(key, max_chars, NULL);
  if(shm_id == -1){
    printf("Failed to create shared memory: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  shm_data = shmat(shm_id, NULL, 0);
  if(shm_data == (char *)(-1)){
    perror("An error occured trying to attach to the shared memory.\n");
    exit(EXIT_FAILURE);
  }

  strcpy(buffer, shm_data);
  shmdt(&shm_data);
  printf("Read \'%s\' from shared memory.\n", buffer);
  if(shmctl(shm_id, IPC_RMID, NULL) == -1){
    perror("An error occured trying to mark shared memory for destruction.\n");
    exit(EXIT_FAILURE);
  }

  free(buffer);
  shmdt(&shm_data);
}

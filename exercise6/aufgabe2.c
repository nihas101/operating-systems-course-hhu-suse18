/* gcc sema_vorgabe.c -o sema_vorgabe -lpthread */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct{
	sem_t *sem_a;
	sem_t *sem_b;
}semaphores;

void *print_stern (void *semaphore_v) {
  semaphores *sems = (semaphores*) semaphore_v;
  char ch = '*';

   while(1) {
	    sem_wait(sems->sem_a); // FIX
      putchar(ch);
      fflush(stdout);
		  sem_post(sems->sem_b); // FIX
  }

  return NULL;
}

void *print_minus (void *semaphore_v) {
	semaphores *sems = (semaphores*) semaphore_v;
   char ch = '-';

   while(1) {
     sem_wait(sems->sem_b); // FIX
     putchar(ch);
     fflush(stdout);
     sem_post(sems->sem_a); // FIX
   }

  return NULL;
}

int main () {
   pthread_t tref1, tref2;
	sem_t *semaphore_a;
  sem_t *semaphore_b;

	semaphore_a = malloc(sizeof(sem_t));
  semaphore_b = malloc(sizeof(sem_t));

	if(sem_init(semaphore_a, 0, 1) != 0){
		printf("An error occured when trying to create the semaphore.\n");
		free(semaphore_a);
    free(semaphore_b);
		return -1;
	}

  if(sem_init(semaphore_b, 0, 0) != 0){
		printf("An error occured when trying to create the semaphore.\n");
    free(semaphore_a);
    free(semaphore_b);
		return -1;
	}

	semaphores *sems = malloc(sizeof(semaphores));
	sems->sem_a = semaphore_a;
	sems->sem_b = semaphore_b;


   if (pthread_create (&tref1, NULL, print_minus, sems)) {
      fprintf(stderr, "error: creating thread 1 failed.\n");
      return EXIT_FAILURE;
   }

   if (pthread_create (&tref2, NULL, print_stern, sems)) {
      fprintf(stderr, "error: creating thread 2 failed.\n");
      return EXIT_FAILURE;
   }

   if (pthread_join (tref1, NULL)) {
      fprintf(stderr, "error: joining thread 1 failed.\n");
      return EXIT_FAILURE;
   }
   if (pthread_join (tref2, NULL)) {
      fprintf(stderr, "error: joining thread 2 failed.\n");
      return EXIT_FAILURE;
   }

	sem_destroy(semaphore_a);
  sem_destroy(semaphore_b);
  free(sems);

   return EXIT_SUCCESS;
}

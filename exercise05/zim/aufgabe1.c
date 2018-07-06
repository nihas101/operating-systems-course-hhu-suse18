#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>


/* second thread */
void *my_thread(void *param) {
   char *zahl_str = (char *)param; /* copy param to our own stack */
   int i;
   int *number = malloc(sizeof(int)); // FIX
   *number = 0;                       // FIX

   for (i=0; i<strlen(zahl_str); i++) {
      *number += ((int)zahl_str[i] - '0');
   }

   return number;
}


int main (int argc, char **argv) {
   pthread_t thread_ref; /* thread reference */
   int *ret; /* pointer to string returned by 2nd thread */

   if (argc<2) {
      printf("error: number expected.\n");
      exit(-1);
   }

   /* create another thread */
   /* (reference, attributes (priority, stacksize, ...), function, params) */
   if ( pthread_create(&thread_ref, NULL, &my_thread, (void*) argv[1]) ) {
      fprintf(stderr, "Error creating thread\n");
      return 1;
   }

  /* wait for the second thread to finish */
   if (pthread_join(thread_ref, &ret)) {
      fprintf(stderr, "Error joining thread\n");
      return 2;
   }

   printf ("cross sum of %s = %d\n", argv[1], *ret);

   free(ret);
}

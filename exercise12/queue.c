#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

void queue_clear(queue_t* queue);

queue_t* queue_new(int capacity){
  queue_t *queue = malloc(sizeof(queue_t));
  if(queue == NULL){
    perror("queue_new: Failed to allocate enough space.\n");
    exit(EXIT_FAILURE);
  }
  queue->head = NULL;
  queue->tail = NULL;
  queue->capacity = capacity;
  queue->size = 0;
  return queue;
}

void queue_delete(queue_t* queue){
  queue_clear(queue);
  free(queue);
}

void queue_clear(queue_t* queue){
  queue_node_t *next;
  queue_node_t *current = queue->head;

  while(current != NULL){
    next = current->next;
    free(current);
    current = next;
  }

  queue->size = 0;
}

int queue_push_back(queue_t* queue, int value){
  if(queue->size >= queue->capacity){
    return -1;
  }

  queue_node_t *node = malloc(sizeof(queue_node_t));
  if(node == NULL){
    perror("queue_push_back: Failed to allocate enough space.\n");
    exit(EXIT_FAILURE);
  }

  node->value = value;
  node->next = NULL;

  if(queue->tail == NULL){     // Is the first element
    queue->head = node;
    queue->tail = node;
  }else{
    queue->tail->next = node;
    queue->tail = node;
  }

  return ++queue->size;
}

int queue_pop_front(queue_t* queue, int* ret_value){
  if(queue->size <= 0){
    return -1;
  }

  queue_node_t *to_del = queue->head;
  *ret_value = to_del->value;

  if(queue->size == 1){
    queue->head = NULL;
    queue->tail = NULL;
  }else{
    queue->head = to_del->next;
  }

  free(to_del);
  queue->size = queue->size-1;
  return queue->size;
}

void queue_print(struct queue* queue){
  if(queue == NULL){
    printf("NULL\n");
    return;
  }

  queue_node_t *current = queue->head;

  printf("Cap: %d\n", queue->capacity);
  printf("Size: %d\n", queue->size);

  if(queue->head != NULL && queue->tail != NULL){
    printf("HEAD: %d\n", queue->head->value);
    printf("TAIL: %d\n", queue->tail->value);
  }else if(queue->head == NULL && queue->tail == NULL){
    printf("HEAD: NULL\n");
    printf("TAIL: NULL\n");
  }

  printf("[");

  while(current != NULL){
    printf(" %d ", current->value);
    current = current->next;
  }

  printf("]\n");
}

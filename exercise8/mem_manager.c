#include <stdlib.h>
#include <stdio.h>
#include "mem_manager.h"

mem_node_t *head;

bool fits(mem_node_t *node, int size);
void print_mem_node(mem_node_t *node);
void insert_new_block(mem_node_t *block, int size);
void try_to_fuse_w_next_block(mem_node_t *block);
void try_to_fuse_w_prev_block(mem_node_t *block);
mem_node_t* find_prev_block(mem_node_t *block);

void ff_init(int size){
  int true_size = size + sizeof(mem_node_t);
  head = malloc(true_size);
  if(head == NULL){
    printf("malloc failed.\n");
    exit(EXIT_FAILURE);
  }

  head->size = size;
  head->allocated = false;
  head->next = NULL;
}

void ff_cleanup(){
  free(head);
}

void* ff_alloc(int size){
  mem_node_t *current = head;
  int true_size = size + sizeof(mem_node_t);

  while(!fits(current, true_size)){
    current = current->next;
  }

  if(current == NULL){
    printf("No fitting block was found.\n");
    exit(EXIT_FAILURE);
  }

  current->allocated = true;
  if(current->size > true_size){ // Allocated block is not a perfect fit
    insert_new_block(current, true_size);
  }else{ // Perfect fitting block was found
    current->allocated = true;
  }

  return ((void*)current) + sizeof(mem_node_t); //Skip header
}

void insert_new_block(mem_node_t *block, int block_size){
  mem_node_t *new;
  new = ((void*)block) + block_size;
  new->size = block->size - block_size;
  new->allocated = false;
  block->size = block->size - new->size;

  if(block->next == NULL){ // Insert at the end
    block->next = new;
    new->next = NULL;
  }else{ // Insert in between two blocks
    new->next = block->next;
    block->next = new;
  }
}

bool fits(mem_node_t *node, int size){
  return node != NULL && !node->allocated && node->size >= size;
}

void ff_free(void* ptr){
  mem_node_t *true_ptr = (mem_node_t*)(ptr - sizeof(mem_node_t));
  true_ptr->allocated = false;

  try_to_fuse_w_next_block(true_ptr);
  try_to_fuse_w_prev_block(true_ptr);
}

void try_to_fuse_w_prev_block(mem_node_t *block){
  mem_node_t *prev = find_prev_block(block);

  if(prev != NULL && prev->next == block){
    try_to_fuse_w_next_block(prev);
  }
}

mem_node_t* find_prev_block(mem_node_t *block){
  mem_node_t *current = head;

  while(current->next != NULL && current->next != block){
    current = current->next;
  }

  return current;
}

void try_to_fuse_w_next_block(mem_node_t *block){
  if(block->next != NULL && !block->next->allocated){
    block->size = block->size + block->next->size;
    block->next = block->next->next;
  }
}

void ff_dump(){
  printf("[H]");
  print_mem_node(head);
  mem_node_t *current = head->next;
  while(current != NULL){
    print_mem_node(current);
    current = current->next;
  }
  printf("NULL \n");
}

void print_mem_node(mem_node_t *node){
  printf("[ Size: %d | Alloc'd: ", node->size);
  printf(node->allocated ? "true ]" : "false ]");
  printf("\n-> ");
}

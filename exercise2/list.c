#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MAX 100
#define DEBUG true
#define BASE 10

struct list_node{
  int value;
  struct list_node *next;
};

typedef struct list_node node_t;

void create_random_node(node_t *node_p);

node_t* add_random_to_list(node_t *head, int number_of_elements);
node_t* remove_random_from_list(node_t *head, int number_of_elements);

node_t* add_element_sorted(node_t *head, int v);
node_t* insert_at_head(node_t *head, int v);
void insert_in_tail(node_t *head, int v);

node_t* remove_element(node_t *head, int v);
void remove_in_tail(node_t *head, int v);

void print_list(node_t *head);
void free_list(node_t *head);
int random_int();

int main(int argc, char *argv[]) {
  if(argc < 3) return 1;
  srand(time(NULL));
  int to_add = strtol(argv[1], NULL, BASE);
  int to_remove = strtol(argv[2], NULL, BASE);

  if(to_remove > to_add) to_remove = to_add;
  if(to_add == 0) return 0;

  node_t* test_list_p = malloc(sizeof(node_t));
  if(test_list_p == NULL) return 1;

  create_random_node(test_list_p);

  if(to_add > 1) test_list_p = add_random_to_list(test_list_p, to_add-1);

  printf("\n");
  print_list(test_list_p);
  printf("\n");

  test_list_p = remove_random_from_list(test_list_p, to_remove);

  printf("\n");
  print_list(test_list_p);
  printf("\n");

  free_list(test_list_p);

  return 0;
}

node_t* add_random_to_list(node_t *head, int number_of_elements){
  for(int i=0; i < number_of_elements;i++){
    printf("%d. ", (i+1));
    head = add_element_sorted(head, random_int());
  }

  return head;
}

node_t* remove_random_from_list(node_t *head, int number_of_elements){
  for(int i=0; i < number_of_elements; i++){
    printf("%d. ", (i+1));
    node_t *current = head;
    while(current->next != NULL){
      if(random_int() > 60) break;
      current = current->next;
    }

    head = remove_element(head, current->value);
  }

  return head;
}

void create_random_node(node_t *node_p){
  node_p->value = random_int();
  node_p->next = NULL;

  if(DEBUG) printf("Created list: [%d]\n", node_p->value);
}

node_t* add_element_sorted(node_t *head, int v){
  node_t *new_head;

  if(DEBUG) printf("Inserting %d\n", v);

  if(head->value >= v){
    new_head = insert_at_head(head, v);
  }else{
    insert_in_tail(head, v);
    new_head = head;
  }

  return new_head;
}

node_t* insert_at_head(node_t *head, int v){
  node_t *node = malloc(sizeof(node_t));
  if(node == NULL) return NULL;

  node->value = v;
  node->next = head;
  return node;
}

void insert_in_tail(node_t *head, int v){
  node_t *current = head;

  while(current->next != NULL && current->next->value < v){
    current = current->next;
  }

  node_t *node = malloc(sizeof(node_t));
  if(node == NULL) return;
  else{
    node->value = v;
    node->next = current->next;
    current->next = node;
  }
}

node_t* remove_element(node_t *head, int v){
  node_t *new_head;

  if(DEBUG) printf("Removing %d\n", v);

  if(head->value == v){
    new_head = head->next;
    free(head);
  }else{
    remove_in_tail(head, v);
    new_head = head;
  }

  return new_head;
}

void remove_in_tail(node_t *head, int v){
  node_t *current = head;

  while(current->next != NULL && current->next->value != v){
    current = current->next;
  }

  if(current->next != NULL){
    node_t *to_delete = current->next;
    current->next = current->next->next;
    free(to_delete);
  }
}

void print_list(node_t *head){
  node_t *current = head;

  printf("[");

  while(current != NULL){
    printf("%d", current->value);
    current = current->next;
    if(current != NULL) printf(", ");
  }

  printf("]\n");
}

int random_int(){
  return rand() % MAX;
}

void free_list(node_t *head){
  if(head != NULL) {
    free_list(head->next);
    free(head);
  }
}

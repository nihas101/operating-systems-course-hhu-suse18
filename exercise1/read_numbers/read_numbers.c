#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define READ_NUMBER_MAX_SIZE 10

typedef struct{
  char* read_number;
  bool error;
}Read_number;

char* create_array(void);
Read_number read_number_from_user(char* array);
bool is_int(char read_character);
void print_number(Read_number read_number);

int main() {
  char* read_number;
  read_number = create_array();
  Read_number res;

  if(read_number != NULL){
    res = read_number_from_user(read_number);
    print_number(res);
  }

  free(read_number);
  return 0;
}

char* create_array(){
  char* array;
  array = malloc(sizeof(char)*READ_NUMBER_MAX_SIZE);

  if(array == NULL) printf("malloc failed!\n");

  return array;
}

Read_number read_number_from_user(char* array){
  Read_number read_number;
  char read_character;

  printf("Type in an integer: ");

  for(int i=0; i < READ_NUMBER_MAX_SIZE; i++){
    read_character = getchar();
    if(is_int(read_character)){
      array[i] = read_character;
    }else if(read_character != '\n'){
      array[i] = '\0';
      read_number.error = true;
      break;
    }else{
      array[i] = '\0';
      break;
    }
  }

  read_number.read_number = array;

  return read_number;
}

bool is_int(char read_character){
  return read_character >= '0' && read_character <= '9';
}

void print_number(Read_number read_number){
  if(!read_number.error) printf("Read number: %s\n", read_number.read_number);
  else printf("An error occured while reading the provided number.\n");
}

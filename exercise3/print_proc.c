#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

void print_memory_info(char *meminfo, char *property_name);
char* create_substring(char *substring_pointer, int substring_length);
char* extract_line(char *input, char *prefix);

#define MAX_READ_BYTES 1500

int main() {
  char *pathname = "/proc/meminfo";
  int file_descriptor;
  char *meminfo = malloc(MAX_READ_BYTES*sizeof(char));
  int read_bytes;

  file_descriptor = open(pathname, O_RDONLY);
  read_bytes = read(file_descriptor, meminfo, MAX_READ_BYTES);

  if(read_bytes == -1){
    printf("An error occured reading from %s\n", pathname);
  }else{
    print_memory_info(meminfo, "MemFree");
    print_memory_info(meminfo, "MemTotal");
    print_memory_info(meminfo, "Cached");
  }

  free(meminfo);

  return 0;
}

void print_memory_info(char *meminfo, char *property_name){
  char *property = extract_line(meminfo, property_name);
  if(property != NULL){
    printf("%s\n", property);
  }
  free(property);
}

char* extract_line(char *input, char *prefix){
  char *prefix_pointer;
  char *end_of_line;
  int line_length;
  char *line = NULL;

  prefix_pointer = strstr(input, prefix);
  end_of_line = strchr(prefix_pointer, '\n');

  if(prefix_pointer == NULL || end_of_line == NULL){
    printf("The substring could not be located");
  }else{
    line_length = end_of_line - prefix_pointer;
    line = create_substring(prefix_pointer, line_length);
  }

  return line;
}

char* create_substring(char *substring_pointer, int substring_length){
  char *substring;
  substring = malloc(substring_length*sizeof(char)+1);

  memcpy(substring, substring_pointer, substring_length);
  *(substring + substring_length) = '\0';

  return substring;
}

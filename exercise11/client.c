#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char const *argv[]) {
  char* data;
  char* rec;
  int server;
  int client;
  struct sockaddr_in server_addr;

  if(argc < 4){
    perror("Please provide a port and data to send.\n");
    exit(EXIT_FAILURE);
  }

  int port = atoi(argv[2]);

  data = malloc(sizeof(argv[3]));
  rec = malloc(sizeof(data));
  if(data == NULL || rec == NULL){
    perror("Failed to allocate memory.\n");
    exit(EXIT_FAILURE);
  }

  strcpy(data, argv[3]);

  client = socket(AF_INET, SOCK_STREAM, 0); // TCP/IP
  if(client < 0){
    perror("An error occured trying to create a socket.\n");
    exit(EXIT_FAILURE);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr(argv[1]);

  client = socket(AF_INET, SOCK_STREAM, 0); // TCP/IP
  if(client < 0){
    perror("An error occured trying to create a socket.\n");
    exit(EXIT_FAILURE);
  }

  server = connect(client, &server_addr, sizeof(server_addr));
  if(server < 0){
    perror("An error occured trying to connect to the server.\n");
    exit(EXIT_FAILURE);
  }

  int sent  = send(client, data, strlen(data), NULL);
  if(sent < 0){
    perror("An error occured trying to recieve bytes.\n");
    exit(EXIT_FAILURE);
  }

  printf("%d\n", sent);

  if(recv(client, rec, strlen(data), NULL) < 0){
    perror("An error occured trying to recieve bytes.\n");
    exit(EXIT_FAILURE);
  }

  printf("Recieved: %s\n", rec);

  free(data);
  free(rec);

  return 0;
}

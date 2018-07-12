#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BACKLOG 2
#define BUF_SIZE 1024

int main(int argc, char const *argv[]) {
  int server;
  int client;
  int recv_bytes;
  struct sockaddr_in server_addr;
  char* data = malloc(BUF_SIZE);
  if(data == NULL){
    perror("Failed to allocate memory.\n");
    exit(EXIT_FAILURE);
  }

  if(argc < 3){
    perror("Please provide a port.\n");
    exit(EXIT_FAILURE);
  }

  int port = atoi(argv[2]);

  server = socket(AF_INET, SOCK_STREAM, 0); // TCP/IP
  if(server < 0){
    perror("An error occured trying to create a socket.\n");
    exit(EXIT_FAILURE);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr(argv[1]);

  if(bind(server, &server_addr, sizeof(server_addr)) < 0){
    perror("An error occured trying to bind the created socket.\n");
    exit(EXIT_FAILURE);
  }

  if(listen(server, BACKLOG) < 0){
    perror("An error occured trying to set the socket to listen for connections.\n");
    exit(EXIT_FAILURE);
  }

  while(1){
    client = accept(server, NULL, NULL);

    recv_bytes = recv(client, data, BUF_SIZE, NULL);
    if(recv_bytes < 0){
      perror("An error occured trying to recieve bytes.\n");
      exit(EXIT_FAILURE);
    }

    printf("Recieved: %s\n", data);

    if(send(client, data, recv_bytes, NULL) < 0){
      perror("An error occured trying to recieve bytes.\n");
      exit(EXIT_FAILURE);
    }

    close(client);
  }

  return 0;
}

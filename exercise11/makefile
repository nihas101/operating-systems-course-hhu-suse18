FLAGS=-Wall -Wextra -pedantic

build: sockets sm

sockets: server client

server:
	gcc $(FLAGS) server.c -o server

client:
	gcc $(FLAGS) client.c -o client

sm:
	gcc $(FLAGS) sm_example.c -o sm_example

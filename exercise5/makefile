FLAGS = -Wextra -Wall -pedantic -std=gnu11

build-run: build run

run:
	sudo ./thread_priority_matrix 500 500 500 500 10

build:
	gcc thread_priority_matrix.c matrix.c -o thread_priority_matrix -pthread

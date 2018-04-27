CC = gcc
FLAGS = -std=gnu11 -Wall -Wextra -pedantic

clean-build: clean fork print_proc

fork:
	$(CC) matrix.c fork.c $(FLAGS) -o fork

print_proc:
	$(CC) print_proc.c $(FLAGS) -o print_proc

clean:
	rm fork ; rm print_proc

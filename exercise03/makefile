CC = gcc
FLAGS = -std=gnu11 -Wall -Wextra -pedantic

build: cross_sum fork print_proc

clean-build: clean cross_sum fork print_proc

cross_sum:
	$(CC) cross_sum.c $(FLAGS) -o cross_sum

fork:
	$(CC) matrix.c fork.c $(FLAGS) -o fork

print_proc:
	$(CC) print_proc.c $(FLAGS) -o print_proc

clean:
	rm cross_sum ; rm fork ; rm print_proc

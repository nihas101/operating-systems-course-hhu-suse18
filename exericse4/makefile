CC = gcc
FLAGS = -std=gnu11 -Wall -Wextra -pedantic

build: build-cross_sum build-thread_search_max

build-thread_search_max:
	$(CC) thread_search_max.c $(FLAGS) -o thread_search_max -pthread

build-cross_sum:
	$(CC) cross_sum_thread.c $(FLAGS) -o cross_sum_thread -pthread

clean:
	rm cross_sum_thread ; rm thread_search_max

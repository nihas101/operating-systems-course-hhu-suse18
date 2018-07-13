FLAGS=-Wall -Wextra -pedantic

build: consprod

consprod:
	gcc $(FLAGS) consprod.c queue.c -o consprod -lpthread

clean:
	rm queue ; rm consprod

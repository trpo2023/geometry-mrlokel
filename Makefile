all: trpo

trpo: trpo.c
	gcc -Wall -Werror -o trpo trpo.c

clean:
	rm trpo

run:
	./trpo
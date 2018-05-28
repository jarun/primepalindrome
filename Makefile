CC = gcc
CFLAGS = -Wall -Wextra -Werror -O3 -march=native

all: primepalindrome

primepalindrome:
	$(CC) $(CFLAGS) -o pp primepalindrome.c
	strip pp

.PHONY: clean
clean:
	rm -f pp

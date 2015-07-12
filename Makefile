CC = gcc
CFLAGS = -W -Wall -Werror -O3

all: primepalindrome

primepalindrome:
	$(CC) $(CFLAGS) -o pp primepalindrome.c -lm
	strip pp

.PHONY: clean
clean:
	rm -f pp

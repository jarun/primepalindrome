CC = gcc
CFLAGS = -Wall -Wextra -Werror -O3

all: primepalindrome

primepalindrome:
	$(CC) $(CFLAGS) -o pp primepalindrome.c -lm
	strip pp

.PHONY: clean
clean:
	rm -f pp

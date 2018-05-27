/*
 * A fast C program to calculate the 1500th prime palindrome with 13 digits.
 *
 * Author: Arun Prakash Jana <engineerarun@gmail.com>
 * Copyright (C) 2015 by Arun Prakash Jana <engineerarun@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with primepalindrome.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define __LIMIT__ 1500
#define __COMPLETE__ 0

typedef unsigned long long ull;

static char ascbuf[32] = {0};
static char const hexbuf[] = "0123456789abcdef";
static int midindex;

static ull fastpow10(int n)
{
	if (n < 0 || n > 16) {
		printf("n = %d\n", n);
		exit(1);
	}

	static ull pow10[] = {
		1,
		10,
		100,
		1000,
		10000,
		100000,
		1000000,
		10000000,
		100000000,
		1000000000,
		10000000000,
		100000000000,
		1000000000000,
		10000000000000,
		100000000000000,
		1000000000000000,
		10000000000000000,
	};

	return pow10[n];
}

/* Convert long to ASCII */
static char *ltoa(ull val, int base, int *len){
	static int i;

	i = 30;

	for (; val && i ; --i, val /= base)
		ascbuf[i] = hexbuf[val % base];

	*len = 30 - i;

	return &ascbuf[++i];
}

#if __COMPLETE__
/* Check if a number (converted to string) is palindrome */
static int ispalin(char *buf, int len)
{
	midindex = (len >> 1) - 1;

	while (midindex >= 0) {
		if (buf[midindex] != buf[len - midindex - 1])
			return 0;

		--midindex;
	}

	return 1;
}
#endif

/* Check if a number (converted to string) is divisible by 3 */
static int isdivisibleby3(char *buf, int len)
{
	static int sum;

	sum = 0;

	while (--len >= 0)
		sum += buf[len] - '0';

	if (sum % 3)
		return 0;

	return 1;
}

/* Check if a number is prime.
   _SKIP_s the check for every odd multiple of 3 */
static int isprime(ull val)
{
	static ull i, root;
	static int j;

	/* Test for divisibility by 2 */
	if (!(val & 0x1))
		return 0;

	i = 3;
	j = 0;
	root = (ull) sqrt(val) + 1;

	for (; i < root; i += 2) {
		/* Trick to skip each 2nd multiple of 3 from 3: 9, 15, 21... */
		if (j == 3) {
			j = 1;
			continue;
		}
		++j;

		if (!(val % i))
			return 0;
	}

	return 1;
}

/* Generate the next palindrome after a palindrome
   123454321 -> 123464321
   123494321 -> 123505321
   123999321 -> 124000421
   ... */
static ull getnextpalin(char *buf, int *len)
{
	midindex = (*len >> 1) - 1;

	/* Handle the case of odd number of digits.
	   If the central digit is 9 reset it to 0
	   and process adjacent digit, otherwise
	   increment it and return */
	if ((*len & 0x1) == 0x1) {
		if (buf[midindex + 1] - '0' == 9)
			buf[midindex + 1] = '0';
		else {
			buf[midindex + 1] += 1;
			return atol(buf);
		}
	}

	/* Adjust other digits */
	while (midindex >= 0) {
		if (buf[midindex] - '0' == 9) {
			buf[midindex] = '0';
			buf[*len - midindex - 1] = '0';
		} else {
			buf[midindex] += 1;
			buf[*len - midindex - 1] += 1;
			return atol(buf);
		}

		--midindex;
	}

	/* We have exhausted numbers in *len digits,
	   increase the number of digits and return
	   the first palindrome of the form 10..0..01 */
	/* return (ull) fastpow10(++(*len)) | 1; */

	/* The last palin will always be of the form 99...99 */
	return atol(buf) + 2;
}

#if __COMPLETE__
/* Generate the next palindrome after a NON-palindrome */
static ull nonpalin2palin(char *buf, int len)
{
	midindex = (len >> 1) - 1;

	while (midindex >= 0) {
		if (buf[midindex] > buf[len - midindex - 1]) {
			/* 126454378 -> 126454621 */
			while (midindex >= 0) {
				buf[len - midindex - 1] = buf[midindex];
				--midindex;
			}

			return atol(buf);
		} else if (buf[midindex] < buf[len - midindex - 1]) {
			static ull orig, nextpalin;

			/* 123454678 -> 123454321 (smaller) */
			orig = atol(buf);

			while (midindex >= 0) {
				buf[len - midindex - 1] = buf[midindex];
				--midindex;
			}

			/* Loop for next palindrome greater than original input */
			while ((nextpalin = getnextpalin(buf, &len)) < orig);

			return nextpalin;
		}

		--midindex;
	}

	/* We should never reach here as
	   the function is deterministic */
	return 0;
}
#endif

int main()
{
	int count = __LIMIT__;
	ull i = 1000000000001;
	//ull i = 999999999999;
	int len = 0, oldlen;
	char *buf = ltoa(i, 10, &len);

	/* Uncomment the following code if starting from
	   a non-palindrome. We started at 1000000000001. */
#if __COMPLETE__
	if (!ispalin(buf, len)) {
		/* Get immediate next palindrome after a non-palindrome */
		i = nonpalin2palin(buf, len);

		/* Need to convert in case the number of digits has changed */
		buf = ltoa(i, 10, &len);
		printf("next palin: %s\n", buf);
	}
#endif

	while (1) {
		/* If number of digits are even, all palindromes are divisible by 11.
		   Get first palindrome of next odd number of digits */
		if (!(len & 0x1)) {
			i = (ull) fastpow10(len) | 1;
			buf = ltoa(i, 10, &len);
			continue;
		}

		/* Check if number is divisible by 5 or 3 */
		if ((buf[len - 1] != '5') && (isdivisibleby3(buf, len) == 0)) {
			if (isprime(i)) {
				if (--count == 0) {
					printf("%s\n", buf);
					return 0;
				} /* else
					printf("%6d. %s\n", count, buf); */
			}
		}

		oldlen = len;

		i = getnextpalin(buf, &len);
		/* Refresh buffer if number of digits has increased */
		if (oldlen != len)
			buf = ltoa(i, 10, &len);
	}

	return 0;
}

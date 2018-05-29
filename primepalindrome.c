/*
 * A fast C program to calculate the 1500th prime palindrome with 15 digits.
 *
 * Implements brute-force and optimized Sieve of Eratosthenes (default) method.
 * Uses GCC-specific hardware-aided ffsll() to check prime bits in bitmap.
 *
 * Benchmark on Intel(R) Core(TM) i7-7500U CPU @ 2.70GHz
 * -----------------------------------------------------
 * 13 digits [result: 1015834385101]
 * - Brute: 3.75user 0.00system 0:03.75elapsed 100%CPU (0avgtext+0avgdata 1544maxresident)k
 * - Sieve: 2.80user 0.00system 0:02.80elapsed 99%CPU (0avgtext+0avgdata 1428maxresident)k
 *
 * 15 digits [result: 100191191191001]
 * - Brute: 36.94user 0.00system 0:36.94elapsed 99%CPU (0avgtext+0avgdata 1208maxresident)k
 * - Sieve: 23.94user 0.00system 0:23.94elapsed 100%CPU (0avgtext+0avgdata 3240maxresident)k
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

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __SIEVE__ 1 /* 0 - Brute Force, 1 - Sieve of Eratosthenes */
#define __COMPLETE__ 0 /* Start from a non-palindrome and generate the next palindrome */

#define __DIGITS__ 15
#define __LIMIT__  1500

typedef unsigned long long ull;

static char const hexbuf[] = "0123456789abcdef";
static char ascbuf[32] = {0};
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
static int midindex;

#if __SIEVE__
static unsigned char *psieve;
static ull max, half_max;

#define __ALIGN_MASK(x,mask)    (((x)+(mask))&~(mask))
#define ALIGN(x,a)              __ALIGN_MASK(x,(typeof(x))(a)-1)

#define PRIME 0
#define COMPOSITE !PRIME

#define ffzll(x) ffsll(~(x))
#endif

static ull fastpow10(int n)
{
	if (n < 0 || n > 16) {
#if __SIEVE__
		if (psieve)
			free(psieve);
#endif
		printf("n = %d\n", n);
		exit(1);
	}

	return pow10[n];
}

static ull sqrt_int_round(ull val)
{
	ull res = 0;
	ull one = (ull)1 << 62; // The second-to-top bit is set: use 1u << 14 for uint16_t type; use 1uL<<30 for uint32_t type

	// "one" starts at the highest power of four <= than the argument.
	while (one > val)
		one >>= 2;

	while (one)
	{
		if (val >= res + one)
		{
			val -= (res + one);
			res += one << 1;
		}

		res >>= 1;
		one >>= 2;
	}

	/* Do arithmetic rounding to nearest integer */
	if (val > res)
		++res;

	return res;
}

#if __SIEVE__
/*
 * Generates a table of prime numbers up to the maximum having digits.
 * All even numbers (including 2) are ignored. Bit n in the bitmap
 * represents the odd number (2n + 1). This enables the table to be
 * half the size of a naive Sieve of Eratosthenes implementation.
 */
static void generate_sieve(int digits)
{
	unsigned char mask = 0x7;
	int count = 0, bytes;

	while (count < digits) {
		max = max * 10 + 9;
		++count;
	}
	//printf("max: %llu\n", max);

	max = sqrt_int_round(max);
	//printf("sqrt: %llu\n", max);
	half_max = max >> 1;

	/* We need half the space as multiples of 2 can be omitted */
	bytes = (max >> 1) + (max & 0x1);
	//printf("%d bits\n", bytes);

	/* Calculate the actual number of bytes required */
	bytes = (bytes >> 3) + (bytes & 0x1);
	//printf("%d bytes\n", bytes);

	/* Align-up to 16-byte */
	bytes = ALIGN(bytes, 16);
	//printf("%d bytes (16-byte aligned)\n", bytes);

	psieve = realloc(psieve, bytes);
	if (!psieve) {
		printf("realloc() failed!\n");
		exit(1);
	}
	memset(psieve, 0, bytes);

	/* In psieve bit 0 -> 1, 1 -> 3, 2 -> 5, 3 -> 7 and so on... */
	/* Set the 0th bit representing 1 to COMPOSITE */
	psieve[0] |= COMPOSITE << (1 >> 1);

	for (ull n = 3; n <= max; n += 2) {
		if (((psieve[n >> 4] >> ((n  >> 1) & mask)) & 0x1) == PRIME) {
			for (ull mul = (n << 1); mul < max; mul += n) {
				/* Skip the evens, there's no representation in psieve */
				if (!(mul & 0x1))
					continue;

				/* Check if already set, reduces performance */
				//if (((psieve[mul >> 4] >> ((mul  >> 1) & mask)) & 0x1) == COMPOSITE)
				//	continue;

				/* Set offset of mul in psieve */
				psieve[mul >> 4] |= COMPOSITE << ((mul >> 1) & mask); /* bit offset */
			}
		}
	}
}
#endif

#if 0
/* Disabled as we are not calling this API */
static void printsieve()
{
	unsigned char mask;
	int count = 0;
	ulong offset;

	printf("prime %d: %u\n", ++count, 2);

	for (ull i = 0; i <= half_max; ++i) {
		offset = i >> 3;
		mask = i & 0x7;
		if (((psieve[offset] >> mask) & 0x1) == PRIME) {
			printf("prime %d: %llu\n", ++count, (i << 1) + 1);
		}
	}
}
#endif

/* Convert long to ASCII */
static char *ltoa(ull val, int base, int *len)
{
	static int i;

	i = 30;

	for (; val && i ; --i, val /= base)
		ascbuf[i] = hexbuf[val % base];

	*len = 30 - i;

	return &ascbuf[++i];
}

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

/* Check if a number is prime. */
static int isprime(ull val)
{
	/* Test for divisibility by 2 */
	if (!(val & 0x1))
		return 0;

#if __SIEVE__
#ifdef __SW_BITMAP_CHECK__
	static unsigned char mask;
	static ulong offset;

	/* We have already checked for divisibility by 2, 3, 5, start from 7 */
	for (ull i = 3; i <= half_max; ++i) {
		offset = i >> 3;
		mask = (unsigned char)i & 0x7;
		if (((psieve[offset] >> mask) & 0x1) == PRIME)
			if (!(val % ((i << 1) + 1)))
				return 0;
	}
#else /* hardware ffs enabled */
	static ull *pquadbits, quad, next, prev, i;
	pquadbits = (ull *)psieve;
	quad = ~*pquadbits & ~0b111;
	prev = 0, next = 3; /* start at 7 (3 * 2 + 1) */

	for (; prev <= half_max; quad = ~*++pquadbits) {
		if (!quad) {
			prev += 64;
			continue;
		}

		while (quad) {
			i = ffsll(quad) - 1;
			next = prev + i;
			if (!(val % ((next << 1) + 1)))
				return 0;
			quad &= ~((ull)0x1 << i);
		}

		prev += 64;
	}
#endif
#else
	/* Brute-force approach to determine prime */
	/* _SKIP_s the check for every odd multiple of 3 and 5 */
	static ull i, root;
	static ulong j, k;

	i = 7; /* divisibility by 3 has already been checked before calling this function */
	j = 1; /* j can be 0, 1, 2; e.g.: 3 - set to 0, 5 - 1, 7 - 2, 9 - j found as 2 and reset */
	k = 0; /* k can be 0, 1, 2, 3, 4; e.g.: 5 - set to 0, 7 - 1, 9 - 2, 11 - 3, 13 - 4, 15 - k found as 4 and reset */
	root = sqrt_int_round(val);

	for (; i < root; i += 2) {
		/* Trick to skip each 2nd multiple of 3 from 3: 9, 15, 21... */
		if (j == 2) {
			j = 0;
			k == 4 ? k = 0 : ++k;
			continue;
		}
		++j;

		/* Trick to skip each 2nd multiple of 5 from 5: 15, 25, 35... */
		if (k == 4) {
			k = 0;
			continue;
		}
		++k;

		if (!(val % i))
			return 0;
	}
#endif

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
	ull i = 100000000000001;
	int len = 0, oldlen;
	char *buf = ltoa(i, 10, &len);

	if (len < __DIGITS__) {
		printf("len: %d\n", len);
		exit(1);
	}

#if __SIEVE__
	generate_sieve(__DIGITS__);
#endif

	/* Enable the following code if starting from
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

#if __SIEVE__
	if (psieve)
		free(psieve);
#endif

	return 0;
}

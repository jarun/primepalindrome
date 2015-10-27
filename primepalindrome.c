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
 * along with Fun-Projects.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define _LIMIT_ 1500

/* Convert long to ASCII */
char *ltoa(long val, int base, int *len){
	static char buf[32] = {0};
	int i = 30;

	for(; val && i ; --i, val /= base)
		buf[i] = "0123456789abcdef"[val % base];

	*len = 30 - i;

	return &buf[i+1];
}

/* Check if a number (converted to string) is palindrome */
int ispalin(char *buf, int len)
{
	int mid = (len >> 1) - 1;

	while (mid >= 0) {
		if (buf[mid] != buf[len - mid - 1])
			return 0;

		mid--;
	}

	return 1;
}

/* Check if a number (converted to string) is divisible by 3 */
int isdivisibleby3(char *buf, int len)
{
	int sum = 0;

	while (--len >= 0)
		sum += buf[len] - '0';

	if (sum % 3)
		return 0;

	return 1;
}

/* Check if a number if prime.  SKIPs the check for 3 due to an
   earlier check done specifically for this program */
int isprime(long val)
{
	/* Test for divisibility by 2 */
	if ((val & 0x1) == 0x0)
		return 0;

	long i = 3;
	long root = (long) sqrt(val) + 1;

	for (; i < root; i += 2) {
		/* Skip multiples of 3 as divisors, we have already
		   checked divisibility by 3 in isdivisibleby3() */
		if (i % 3 == 0)
			continue;

		if (!(val % i))
			return 0;
	}

	return 1;
}

/* Generate the next palindrome after a palindrome */
long getnextpalin(char *buf, int *len)
{
	int mid = (*len >> 1) - 1;

	if ((*len & 0x1) == 0x1) {
		if (buf[mid + 1] - '0' == 9)
			buf[mid + 1] = '0';
		else {
			buf[mid + 1] += 1;
			return atol(buf);
		}
	}

	while (mid >= 0) {
		if (buf[mid] - '0' == 9) {
			buf[mid] = '0';
			buf[*len - mid - 1] = '0';
		} else {
			buf[mid] += 1;
			buf[*len - mid - 1] += 1;
			return atol(buf);
		}

		mid--;
	}

	/* We have exhausted numbers in *len digits,
	   increase the number of digits and return
	   the first palindrome of the form 10..0..01 */
	(*len)++;
	return (long) pow(10, *len - 1) | 1;
}

/* Generate the next palindrome after a NON-palindrome */
long nonpalin2palin(char *buf, int len)
{
	long orig;
	long nextpalin;
	int mid = (len >> 1) - 1;

	while (mid >= 0) {
		if (buf[mid] > buf[len - mid - 1]) {
			while (mid >= 0) {
				buf[len - mid - 1] = buf[mid];
				mid--;
			}

			return atol(buf);
		} else if (buf[mid] < buf[len - mid - 1]) {
			orig = atol(buf);

			while (mid >= 0) {
				buf[len - mid - 1] = buf[mid];
				mid--;
			}

			/* Loop for next palindrome greater than original input */
			while ((nextpalin = getnextpalin(buf, &len)) < orig);

			return nextpalin;
		}

		mid--;
	}

	/* We should never reach here as
	   the function is deterministic */
	return 0;
}

int main()
{
	int count = 0;
	char *buf = NULL;
	long i = 1000000000001;
	//long i = 999999999999;
	int len = 0, oldlen;

	int ret = system("date");
	ret++;

	buf = ltoa(i, 10, &len);

	if (!ispalin(buf, len)) {
		/* Get immediate next palindrome after a non-palindrome */
		i = nonpalin2palin(buf, len);

		/* Need to convert in case the number of digits has changed */
		buf = ltoa(i, 10, &len);
		printf("next palin: %s\n", buf);
	}

	while (1) {
		/* If number of digits are even, all palindromes are divisible by 11.
		   Get first palindrome of next odd number of digits */
		if ((len & 0x1) == 0x0) {
			i = (long) pow(10, len) | 1;
			buf = ltoa(i, 10, &len);
			continue;
		}

		if ((buf[len - 1] != '5') && (isdivisibleby3(buf, len) == 0)) {
			if (isprime(i)) {
				if (++count == _LIMIT_) {
					ret = system("date");
					ret++;
					printf("%6d. %s\n", count, buf);
					return 0;
				} /* else
					printf("%6d. %s\n", count, buf); */
			}
		}

		oldlen = len;

		i = getnextpalin(buf, &len);
		/* Refresh buffer if number of digits has changed */
		if (oldlen != len)
			buf = ltoa(i, 10, &len);
	}

	return 0;
}

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

#define LIMIT 1500

char *ltoa(long val, int base, int *len){
	static char buf[32] = {0};
	int i = 30;

	for(; val && i ; --i, val /= base)
		buf[i] = "0123456789abcdef"[val % base];

	*len = 30 - i;

	return &buf[i+1];
}

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

int isprime(long val)
{
	/* Test for divisibility by 2 */
	if ((val & 0x1) == 0x0)
		return 0;

	long i = 3;
	long root = (long) sqrt(val) + 1;

	for (; i < root; i += 2)
		if (!(val % i))
			return 0;

	return 1;
}

long getnextpalin(char *buf, int len)
{
	int mid = (len >> 1) - 1;

	if ((len & 0x1) == 0x1) {
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
			buf[len - mid - 1] = '0';
		} else {
			buf[mid] += 1;
			buf[len - mid - 1] += 1;
			return atol(buf);
		}

		mid--;
	}

	/* We have exhausted numbers in len digits,
	   increase the number of digits and return
	   the first palindrome of the form 10..0..01 */
	return (long) pow(10, len) | 1;
}

int main()
{
	int count = 0;
	char *buf = NULL;
	long i = 1000000000001;
	int len = 0;

	int ret = system("date");
	ret++;

	while (1) {
		buf = ltoa(i, 10, &len);

		/* If number of digits are even, all palindromes are divisible by 11.
		   Get first palindrome of next odd number of digits */
		if ((len & 0x1) == 0x0) {
			i = (long) pow(10, len) | 1;
			continue;
		}

		if (buf[len - 1] != '5') {
			if (isprime(i)) {
				if (++count == LIMIT) {
					ret = system("date");
					ret++;
					printf("%6d. %s\n", count, buf);
					return 0;
				} /* else
					printf("%6d. %s\n", count, buf); */
			}
		}

		i = getnextpalin(buf, len);
	}

	return 0;
}

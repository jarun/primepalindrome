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

int isprime(long val)
{
	/* Test for divisibility by 2 */
	/* if ((val & 0x1) == 0x0)
		return 0; */

	long i = 3;
	long root = (long) sqrt(val) + 1;

	for (; i < root; i += 2)
		if (!(val % i))
			return 0;

	return 1;
}

char* ltoa(long val, int base){
	static char buf[32] = {0};
	int i = 30;

	for(; val && i ; --i, val /= base)
		buf[i] = "0123456789abcdef"[val % base];

	return &buf[i+1];
}

int main()
{
	int count = 0;
	char* buf = NULL;
	long i = 1000000000001;

	int ret = system("date");
	ret++;

	for(; i < 9999999999999; i += 2) {
		buf = ltoa(i, 10);

		if (buf[12] == buf[0] && buf[11] == buf[1]
			&& buf[10] == buf[2] && buf[9] == buf[3]
			&& buf[8] == buf[4] && buf[7] == buf[5]
			&& buf[12] != '5') {
			if (isprime(i)) {
				if (++count == 1500) {
					ret = system("date");
					ret++;
					printf("count: %d val: %s\n", count, buf);
					return 0;
				}
			}
		}
	}

	return 0;
}

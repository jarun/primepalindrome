# PrimePalindrome

A fast C program to calculate the 1500th prime palindrome with an odd number (default 15) of digits.
Why _odd_? Because a palindromic number with even number of digits is always divisible by 11.

Implements space-optimized Sieve of Eratosthenes (default) with GCC-specific hardware-aided `ffsll()`as well as brute force methods.

This program is also a demonstration of using efficient algorithms and C code-optimization.

## Execution time (brute force)

/* On Intel(R) Core(TM) i5-3210M CPU @ 2.50GHz */

**Without logic to generate next palindrome (13 digits)**

    $ ./pp
    Thu Jul  2 00:28:26 IST 2015
    Thu Jul  2 00:32:17 IST 2015
    count: 1500 val: 1015834385101
    3 minutes 51 seconds

**With logic to generate next palindrome (13 digits)**

    $ ./pp
    Tue Oct 27 23:40:41 IST 2015
    Tue Oct 27 23:40:48 IST 2015
    1500. 1015834385101
    *7 seconds!!!*

**With tweaked (but brute-force) prime detection algorithm (13 digits)**

    $ time ./pp
    Sat Mar  5 00:04:23 IST 2016
    Sat Mar  5 00:04:29 IST 2016
    1015834385101
    5.86user 0.00system 0:05.86elapsed 100%CPU (0avgtext+0avgdata 2020maxresident)k
    0inputs+0outputs (0major+420minor)pagefaults 0swaps

/* On Intel(R) Core(TM) i7-7500U CPU @ 2.70GHz */

    // Sieve of Eratosthenes
    13 digits: 2.80user 0.00system 0:02.80elapsed 99%CPU (0avgtext+0avgdata 1428maxresident)k
    15 digits: 23.94user 0.00system 0:23.94elapsed 100%CPU (0avgtext+0avgdata 3240maxresident)k

    // Brute force
    13 digits: 3.75user 0.00system 0:03.75elapsed 100%CPU (0avgtext+0avgdata 1544maxresident)k
    15 digits: 36.94user 0.00system 0:36.94elapsed 99%CPU (0avgtext+0avgdata 1208maxresident)k

## License & Copyright

GPLv3.0

Copyright (C) 2015 [Arun Prakash Jana](mailto:engineerarun@gmail.com)

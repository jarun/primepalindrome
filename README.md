# PrimePalindrome  
  
A fast C program to calculate the 1500th prime palindrome with 13 digits.  
Why 13 digits? Because a palindromic number with even number of digits is always divisible by 11.
  
# License  
  
GPLv3.0

# Execution time  
  
/* On Intel(R) Core(TM) i5-3210M CPU @ 2.50GHz */  
  
**Without logic to generate next palindrome**  

    $ ./pp  
    Thu Jul  2 00:28:26 IST 2015  
    Thu Jul  2 00:32:17 IST 2015  
    count: 1500 val: 1015834385101
    3 minutes 51 seconds  
  
**With logic to generate next palindrome**  

    $ ./pp
    Tue Oct 27 23:40:41 IST 2015
    Tue Oct 27 23:40:48 IST 2015
    1500. 1015834385101
    *7 seconds!!!*
    
**Latest iteration**

    $ time ./pp
    Sat Mar  5 00:04:23 IST 2016
    Sat Mar  5 00:04:29 IST 2016
    1015834385101
    5.86user 0.00system 0:05.86elapsed 100%CPU (0avgtext+0avgdata 2020maxresident)k
    0inputs+0outputs (0major+420minor)pagefaults 0swaps

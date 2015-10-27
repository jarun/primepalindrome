# PrimePalindrome  
  
A fast C program to calculate the 1500th prime palindrome with 13 digits.  
Why 13 digits? Because a palindromic number with even number of digits is always divisible by 11.
  
# Execution time  
  
/* On Intel(R) Core(TM) i5-3210M CPU @ 2.50GHz */  
  
$ ./pp  
Thu Jul  2 00:28:26 IST 2015  
Thu Jul  2 00:32:17 IST 2015  
count: 1500 val: 1015834385101  
  
# License  
  
GPLv3.0  
  
# Notes  
  
It's possible to optimize further by using an algorithm to find the next palindrome. Ref:   
  
https://vchenna.wordpress.com/2013/02/10/find-next-palindrome-number/

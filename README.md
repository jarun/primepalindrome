# PrimePalindrome  
  
A fast C program to calculate the 1500th prime palindrome with 13 digits.  
Why 13 digits? Because a palindromic number with even number of digits is always divisible by 11.
  
# License  
  
GPLv3.0

# Execution time  
  
/* On Intel(R) Core(TM) i5-3210M CPU @ 2.50GHz */  
  
<b>Without logic to generate next palindrome</b>  
<pre>$ ./pp  
Thu Jul  2 00:28:26 IST 2015  
Thu Jul  2 00:32:17 IST 2015  
count: 1500 val: 1015834385101</pre>
3 minutes 51 seconds  
  
<b>With logic to generate next palindrome</b>  
<pre>$ ./pp
Tue Oct 27 20:47:32 IST 2015
Tue Oct 27 20:47:41 IST 2015
  1500. 1015834385101</pre>
<i>9 seconds!!!</i>

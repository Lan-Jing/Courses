# [Loop] The Binary Equivalent (有符号数的二进制显示)

# Description
Write a program that read in several integers, and prints the binary equivalent of those numbers. The first bit encodes the sign.
# Input
Several integers, the value of each of the input integers are in the range of [-32768, 32767].   
One integer on each line. Use EOF to end your input.
# Output
The binary equivalent of those decimal numbers. The outputted binary number takes 16 bits and 0’s should be placed as prefix if the most significant bit is not at $2^{16}$.   
For negative numbers, shown its Two's complement (补码). That is, turn its absolute value to binary format, then inverting all the bits and adding 1 to that result. 
For example, -1’s two complement = invert (0000000000000001) + 1 = 1111111111111110 + 1 = 1111111111111111

# Sample Input
```
255
1
-1
-127
```

# Sample Output

```
0000000011111111
0000000000000001
1111111111111111
1111111110000001
```
# Hint
 There are mutiple test cases ended with EOF.  
You can deal with it as the following code:
```c
//First read somehting from input.
while(scanf("...") != EOF) {
  //Read the remaining input.
  ....
  //Deal with one test case
  ...
  //Output the result.
  ...
}
```
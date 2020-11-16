# 【Program】Function Point

## Description
A function pointer, also called a subroutine pointer or procedure pointer, is a pointer that points to a function. As opposed to referencing a data value, a function pointer points to executable code within memory.Function pointers can be used to simplify code by providing a simple way to select a function to execute based on run-time values.

In this program, you need to finish function `PF maxmin(int c)` where `typedef int (*PF)(int,int)`. In `maxmin` function, the program will return `max` funtion if c == 1, return `min` function otherwise.

## Input
Two numbers a, b and a control number `c`.

## Output
if c == 1

output the maximum in a, b

if c != 1

output the minimum in a, b

## Example input
```
4 2
1
```


## Example output
```
4
```

## Hint
[Function pointer](https://en.wikipedia.org/wiki/Function_pointer)

不靠谱博客：[C/C++ 函数指针使用总结](https://www.cnblogs.com/lvchaoshun/p/7806248.html)
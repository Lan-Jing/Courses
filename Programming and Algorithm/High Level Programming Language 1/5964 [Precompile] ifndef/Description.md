# 【Precompile】ifndef

## Description
c语言中头文件(.h)往往会用到预编译命令 ,使用`#ifndef`可以避免下面这种错误：如果在h文件中定义了全局变量，一个c文件包含同一个h文件多次，如果不加`#ifndef`宏定义，会出现变量重复定义的错误.
swap.h文件

```c
#ifndef SWAP_H
#define SWAP_H

// you code

#endif
```

将代码用 `#ifndef` `#define` `#endif` 包裹的目的是，防止头文件被重复 `#include` 导致头文件内函数，变量的重复声明。
下面我们用一个例子说明 `#ifndef` `#define` `#endif` 的作用。请认真理解 **main.c swap.h** 之间关系

**只需要提交 swap.h, 请勿提交main函数**

## Input
两个整数a, b

## Output
交换后的a,b

## Example input
```
9323 123
```

## Example ouput
```
Before swap: a=9323, b=123
After swap: a=123, b=9323
```

## Hint
条件指示符`#ifndef` 的最主要目的是防止头文件的重复包含和编译。

使用`#ifndef`可以避免下面这种错误：如果在h文件中定义了全局变量，一个c文件包含同一个h文件多次，如果不加`#ifndef`宏定义，会出现变量重复定义的错误；如果加了`#ifndef`，则不会出现这种错误。
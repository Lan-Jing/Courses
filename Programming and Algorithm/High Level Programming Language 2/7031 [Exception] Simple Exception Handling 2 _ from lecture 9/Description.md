# [Exception] Simple Exception Handling 2 _ from lecture 9

## Description
函数f3可能会抛出三种异常类型，double，float，int。你需要在f.cpp中实现两个函数f1和f2，并在f2中调用f3，在f1中调用f2。f2中要处理int和float异常，f1中要对三种异常都进行处理。处理方式为 比如f1捕获了int类型的异常，则输出"An int exception occurred!--from f1"，f2异常处理方式类似。如果在f1中没有异常出现，则输出"End of f1"，f2类似。
f1,f2的声明如下：
```
void f1(int);
void f2(int);
```

函数调用关系如下：

![file](/api/users/image?path=7707/images/1556890775916.png)
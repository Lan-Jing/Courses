# [Classes and Objects] The Student Class

## Description
```
 With a class "Student" declared as below:
 class Student {
public:
  int id;
  char name[50]; // Data field
  int age; // Data field
  Student();
  Student(int, char*, int);
};
 
You are required to implement the class functions and also two other functions used to process Student objects, which can get output as specified later.
 
void set(Student &, int, char*, int);
void print(Student);
 
1) You don't need to submit the main function.
2) You don't need to include the header file for class declaration by yourself.
```
## Input
```

```
## Output
 ```
Steven Gates (100) is 61 years old.
Larry Jordan (123) is 18 years old.
No Name (124) is 0 years old.

```
## 提示，头文件请包涵如下代码：
```
#include<iostream>
#include<cstring>
using namespace std;

class Student
{
public:
  int id;
  char name[50]; // Data field
  int age; // Data field
  Student();
  Student(int, char*, int);
  //void set(int, char*, int);
  //void print();
};

void set(Student &, int, char*, int);
void print(Student);
```

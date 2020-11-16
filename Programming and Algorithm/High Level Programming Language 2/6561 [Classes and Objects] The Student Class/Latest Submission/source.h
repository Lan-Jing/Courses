#include<iostream>
#include<cstring>
using namespace std;

class Student
{
public:
  int id;
  char name[50]; // Data field
  int age; // Data field
  Student(int id = 0, char* name = "No Name",int age = 0);
  //void set(int, char*, int);
  //void print();
};

void set(Student &, int, char*, int);
void print(Student);

Student::Student(int id_,char* name_,int age_){
    id = id_,age = age_;
    strcpy(name,name_);
}
void set(Student &Stu,int id_,char* name_,int age_){
    Stu.id = id_,Stu.age = age_;
    strcpy(Stu.name,name_);
}
void print(Student Stu){
    printf("%s (%d) is %d years old.\n",Stu.name,Stu.id,Stu.age);
}
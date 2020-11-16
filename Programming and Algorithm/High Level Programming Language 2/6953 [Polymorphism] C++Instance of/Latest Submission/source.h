#include<iostream>
using namespace std;
#include<cstring>

class Object {
	public:
		Object(){ this->name = "Object"; }
		virtual string className(){ return this->name; }
	private:
		string name;
};
class Person : public Object {
	public:
		Person(){ this->name = "Person"; }
		string className(){ return this->name; }
	private:
		string name;
};
class Vehicle : public Object {
	public:
		Vehicle(){ this->name = "Vehicle"; }
		string className(){ return this->name; }
	private:
		string name;
};
class Animal : public Object {
	public:
		Animal(){ this->name = "Animal"; }
		string className(){ return this->name; }
	private:
		string name;
};
class Teacher : public Person {
	public:
		Teacher(){ this->name = "Teacher"; }
		string className(){ return this->name; }
	private:
		string name;
};
class Student : public Person {
	public:
		Student(){ this->name = "Student"; }
		string className(){ return this->name; }
	private:
		string name;
};
class Bus : public Vehicle {
	public:
		Bus(){ this->name = "Bus"; }
		string className(){ return this->name; }
	private:
		string name;
};
class Car : public Vehicle {
	public:
		Car(){ this->name = "Car"; }
		string className(){ return this->name; }
	private:
		string name;
};
class Cat : public Animal {
	public:
		Cat(){ this->name = "Cat"; }
		string className(){ return this->name; }
	private:
		string name;
};
class Dog : public Animal {
	public:
		Dog(){ this->name = "Dog"; }
		string className(){ return this->name; }
	private:
		string name;
};
string instanceof(Object& obj){
	return obj.className();
}
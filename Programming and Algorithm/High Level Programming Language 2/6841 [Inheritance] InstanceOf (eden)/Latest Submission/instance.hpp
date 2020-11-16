#include<iostream>
using namespace std;
#include<cstring>

class Object {
    private:
        string type_name = "Object";
    public:
        virtual string name() const { return this->type_name; }
};
class Animal : public Object {
    private:
        string type_name = "Animal";
    public:
        virtual string name() const { return this->type_name; }
};
class Dog : public Animal {
    private:
        string type_name = "Dog";
    public:
        virtual string name() const { return this->type_name; }
};
class Cat : public Animal {
    private:
        string type_name = "Cat";
    public:
        virtual string name() const { return this->type_name; }
};
class Vehicle : public Object {
    private:
        string type_name = "Vehicle";
    public:
        virtual string name() const { return this->type_name; }
};
class Bus : public Vehicle {
    private:
        string type_name = "Bus";
    public:
        virtual string name() const { return this->type_name; }
};
class Car : public Vehicle {
    private:
        string type_name = "Car";
    public:
        virtual string name() const { return this->type_name; }
};
string instanceOf(Object* ptr){
    if(ptr == NULL) return "NULL";
    else return ptr->name();
}
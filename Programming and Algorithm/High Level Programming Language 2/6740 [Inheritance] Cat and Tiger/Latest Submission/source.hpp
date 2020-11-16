#include<iostream>
using namespace std;

class Cat{
    protected:
        int data;
    public:
        Cat(){
            this->data = 0;
            cout<<"Default Constructor of Cat is Running"<<endl;
        }
        Cat(int data){
            this->data = data;
            cout<<"Constructor of Cat is Running "<<data<<endl;
        }
        ~Cat(){
            cout<<"Destructor of Cat is Running "<<this->data<<endl;
        }
};
class Tiger : public Cat {
    private:
        int data;
    public:
        Tiger():Cat() {
            this->data = 0;
            cout<<"Default Constructor of Tiger is Running"<<endl;
        }
        Tiger(int data): Cat(data) {
            this->data = data;
            cout<<"Constructor of Tiger is Running "<<data<<endl;
        }
        ~Tiger(){
            cout<<"Destructor of Tiger is Running "<<this->data<<endl;
        }
};
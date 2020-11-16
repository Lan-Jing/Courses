#include <iostream>
using namespace std;

class base {
    public: virtual void f() { cout<<"f"<<endl; }
};

int main(){
    typedef void (*ptrTof)(void);
    base test;
    cout<<"the address of the object test is:"<<(&test)<<endl;
    cout<<"the address of the virtual function table is:"<<(int*)*(int*)(&test)<<endl;
    ptrTof ptr = (ptrTof)*((int*)*(int*)(&test));
    ptr();
    return 0;
}
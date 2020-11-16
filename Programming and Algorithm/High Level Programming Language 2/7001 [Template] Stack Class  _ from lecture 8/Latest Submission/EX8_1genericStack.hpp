#include<iostream>
using namespace std;
#include<cstring>

template <typename ElementType>                      
class Stack {
    public:
        Stack();
        ~Stack();
        void push(ElementType obj)      throw(std::bad_alloc);
        void pop()                      throw(std::logic_error);
        ElementType getTop() const      throw(std::logic_error);
        bool isEmpty() const;
    private:
        struct Node { 
            ElementType element;     
            Node* next;             
        };
        Node* top;                  
};

template <typename ElementType>
Stack<ElementType>::Stack(){
    this->top = NULL;
}

template <typename ElementType>
Stack<ElementType>::~Stack(){
   while(!this->isEmpty()) this->pop();
   this->top = NULL;
}

template <typename ElementType>
void Stack<ElementType>::push(ElementType obj) throw(std::bad_alloc) {
    Node *temp = new Node;
    temp->element = obj;
    temp->next = this->top;
    this->top = temp;
}

template <typename ElementType>
void Stack<ElementType>::pop() throw(std::logic_error) {
    if(this->isEmpty()) return ;
    Node *temp = this->top->next;
    delete this->top;
    this->top = temp;
}

template <typename ElementType>
ElementType Stack<ElementType>::getTop() const throw(std::logic_error) {
    return this->top->element;
}

template <typename ElementType>
bool Stack<ElementType>::isEmpty() const {
//    cout<<"hellow"<<endl;
    return this->top == NULL ? true : false;
}
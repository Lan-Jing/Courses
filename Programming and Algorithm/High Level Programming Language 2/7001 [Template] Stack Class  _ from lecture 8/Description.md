# [template] Stack Class  _ from lecture 8

## Description
Implement the following template:
```
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
```
## Output
```
8 7 6 5 4 3 2 1

```
#include "EX8_1genericStack.hpp"

using namespace std;

int main() {                 
	Stack<int> stack;	

	for (int i = 1; i < 9; i++) 
		stack.push(i);

	while (!stack.isEmpty()) {	
		cout << stack.getTop() << " "; 
		stack.pop();			   
	}

	return 0;
}

#include <iostream>
#include "Stack.hpp"
using namespace std;

void TestPushAndPop(Stack& s) {
  static int num = 0;
  for (int i = 0; i < 5; i++) {
    s.push(num++);
    cout << "push " << num << endl;
  }

  for (int i = 0; i < 7; i++) {
    if (s.empty()) {
      cout << "stack is empty!" << endl;
    } else {
      cout << "pop " << s.top() << endl;
      s.pop();
    }
  }

  for (int i = 0; i < 5; i++) {
    s.push(num++);
    cout << "push " << num << endl;
  }
}


int main() {
    Stack s1;

    TestPushAndPop(s1);

    Stack s2 = s1;

    s1.clear();

    TestPushAndPop(s2);

    s2 = s2;

    Stack s3(s2);

    s3.clear();

    while (!s2.empty()) {
      cout << "pop " << s2.top() << endl;
      s2.pop();
    }


    return 0;
}
#ifndef STACK_HPP
#define STACK_HPP

#include <stddef.h>

class Stack {
  struct node {
    int num;
    node* next;
    node() { num = 0; next = NULL; }
    node(int n, node* p = NULL) { num = n; next = p; }
  };

  struct {
    node* data;
    int* count;
  } p;

  void copyOnWrite();
public:
  Stack();                        // 构造一个空栈
  Stack(const Stack&);            // 写时复制规则
  ~Stack();                       // 注意内存回收

  Stack operator=(const Stack&);  // 写时复制规则
  void push(int);                 // 入栈
  void pop();                     // 出栈
  int top() const;                // 查看栈顶元素
  bool empty() const;             // 判断栈是否为空
  void clear();                   // 清空栈
};

#endif
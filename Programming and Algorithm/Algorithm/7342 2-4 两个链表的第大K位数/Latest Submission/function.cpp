#include <iostream>
using namespace std;
#include <queue>
#include <vector>
#include "function.h"

bool operator<(const Node a, const Node b) { return a.value < b.value; }
// 写在结构体外面的重载方式，注意参数有两个Node进行比较。

Node answer(0);
priority_queue<Node> que;
Node* FindKthBigElementInTwoList(Node* List1, Node* List2, int k) {
  Node* ptr = List1;
  while (ptr != nullptr) {
    que.push(*ptr);
    ptr = ptr->next;
  }
  ptr = List2;
  while (ptr != nullptr) {
    que.push(*ptr);
    ptr = ptr->next;
  }
  for (int i = 0; i < k; i++) {
    if (i != k - 1)
      que.pop();
    else {
      answer = que.top();
      break;
    }
  }
  return &answer;
}
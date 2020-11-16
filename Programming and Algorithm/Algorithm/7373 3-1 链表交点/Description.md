# 3-1链表交点

## 问题描述:<br>
找到两个链表的相交节点<br><br>
```
struct linkNode {
     int val;
     linkNode *next;
     linkNode(int x) : val(x), next(NULL) {}
 };
```
你所需实现的函数为：
```
linkNode *getIntersectionNode(linkNode *headA, linkNode *headB);
```
返回节点
请记得将头文件包含进去，即#include"linkNode.h" 。
## 输入样例<br>
8->3-> 2->1<br>
6->5-> 4->3-> 2->1<br>
## 输出样例<br>
3<br>
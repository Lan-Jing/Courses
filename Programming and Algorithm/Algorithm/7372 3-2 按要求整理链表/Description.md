# 3-2按要求整理链表

## 问题描述:<br>
给定一个链表和一个值X，操作使得节点值小于X的节点都在大于等于X的节点的前面，并保持每个分区节点的相对顺序不变<br><br>
```
struct linkNode {
     int val;
     linkNode *next;
		 linkNode(){
			val=0;
			next=NULL;
		}
     linkNode(int x) : val(x), next(NULL) {}
 };
```
你所需实现的函数为：
```
linkNode* partList(linkNode* head, int x);
```
返回操作后的带值头结点。
请记得将头文件包含进去，即#include"linkNode.h" 。
## 输入样例<br>
3<br>
6->5-> 4->3-> 2->1<br>
## 输出样例<br>
2->1-> 6->5-> 4->3<br>
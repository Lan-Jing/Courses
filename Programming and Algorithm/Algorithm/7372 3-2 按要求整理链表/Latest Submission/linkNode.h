#ifndef linkNode_h
#define linkNode_h

struct linkNode {
     int val;
     linkNode *next;
     linkNode(){
			val=0;
			next=NULL;
		}
     linkNode(int x) : val(x), next(NULL) {}
     ~linkNode(){}
 };

linkNode *partList(linkNode *head, int x);
#endif

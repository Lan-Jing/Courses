#ifndef linkNode_h
#define linkNode_h

 struct linkNode {
     int val;
     linkNode *next;
     linkNode(int x) : val(x), next(NULL) {}
 };

linkNode *getIntersectionNode(linkNode *headA, linkNode *headB);


#endif /* linkNode_h */

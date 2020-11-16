#include <iostream>
using namespace std;
#include "linkNode.h"
#include <cstring>

linkNode *getIntersectionNode(linkNode *headA,linkNode *headB){
    int numbera[10000]; memset(numbera,0,sizeof(numbera));
    int numberb[10000]; memset(numberb,0,sizeof(numberb));

    linkNode *ptr = headA;
    int ia = 0,iaa = 0;
    while(ptr != nullptr) numbera[ia++] = ptr->val, ptr = ptr->next;
    ptr = headB;
    int ib = 0;
    while(ptr != nullptr) numberb[ib++] = ptr->val, ptr = ptr->next;
    int ansLoc = 0;
    ia--, ib--;
    iaa = ia;
    while(1){
        if(numbera[ia] == numberb[ib]){
            ansLoc++;
            ia--, ib--;
        }else{
            break;
        }
    }
    ptr = headA;
    ansLoc = iaa - ansLoc + 1;
    for(int i = 0;i < ansLoc;i++) ptr = ptr->next; 
    return ptr;
}
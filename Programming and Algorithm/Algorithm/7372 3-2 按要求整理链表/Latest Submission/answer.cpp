#include <iostream>
using namespace std;
#include "linkNode.h"
#include <cstring>

linkNode *partList(linkNode *head, int x){
    int smallNumber[10000]; memset(smallNumber,0,sizeof(smallNumber));
    int largeNumber[10000]; memset(largeNumber,0,sizeof(largeNumber));
    linkNode *ptr = head;
    int is = 0,il = 0;
    while(ptr != nullptr){
        if(ptr->val < x) smallNumber[is++] = ptr->val;
        else largeNumber[il++] = ptr->val;
        ptr = ptr->next;
    }
    linkNode *newHead = nullptr;
    for(int i = 0;i < is;i++){
        if(newHead == nullptr){
            newHead = new linkNode(smallNumber[i]);
            ptr = newHead;
        }else{
            ptr->next = new linkNode(smallNumber[i]);
            ptr = ptr->next;
        }
    }
    for(int i = 0;i < il;i++){
        if(newHead == nullptr){
            newHead = new linkNode(largeNumber[i]);
            ptr = newHead;
        }else{
            ptr->next = new linkNode(largeNumber[i]);
            ptr = ptr->next;
        }
        
    }
    return newHead;
}
#include<stdio.h>
#include<stdlib.h>
#include"linkedList.h"

bool insert(int position,int value){
    if(position < 0 || position > size) return 0;
    else size++;

    node *temp = (node*)malloc(sizeof(node));
    temp->value = value;

    if(!position){
        temp->next = head; head = temp;
    }else{
        int count = 0; node *ptr = head;
        while(count < position-1) count++,ptr = ptr->next;

        temp->next = ptr->next; ptr->next = temp; 
    }
    return 1;
}

int get(int position){
    int count = 0; node *ptr = head;
    while(count < position) ptr = ptr->next,count++;
    return ptr->value;
}

void clear(){
    node *p1 = head,*p2 = NULL;
    while(p1 != NULL){
        p2 = p1->next;
        free(p1);
        p1 = p2;
    }
}
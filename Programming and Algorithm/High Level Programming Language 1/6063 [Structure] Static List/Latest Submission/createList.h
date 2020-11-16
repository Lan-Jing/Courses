#ifndef CREATELIST_H
#define CREATELIST_H

typedef struct node {
  int num;
  struct node* next;
}node;

node* createList(node array[], int n);

#endif
#include "createList.h"
#include <stdio.h>

node* createList(node array[], int n) {
  for (int i = 0; i + 1 < n; i++) {
    array[i].next = &array[i+1];
  }

  array[n-1].next = NULL;
  return &array[0];
}
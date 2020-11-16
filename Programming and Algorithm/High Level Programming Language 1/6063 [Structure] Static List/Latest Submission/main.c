#include <stdio.h>
#include "createList.h"

int array[100100] = {0};

struct node list[1010];

int main() {
  int n;
  scanf("%d", &n);
  for (int i = 0; i < n; i++) {
    scanf("%d", &list[i].num);
    list[i].next = NULL;
  }

  node* head = createList(list, n);

  while (head != NULL) {
    printf("%d ", head->num);
    head = head->next;
  }
  printf("\n");

  return 0;
}
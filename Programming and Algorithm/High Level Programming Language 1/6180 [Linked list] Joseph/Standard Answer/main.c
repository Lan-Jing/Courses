#include <stdio.h>
#include <stdlib.h>

struct node {
  int id;
  struct node* next;
  struct node* prev;
};

struct node* createList(int n) {
  struct node* head = NULL;

  head = (struct node*)malloc(sizeof(struct node));
  head->id = 1;
  head->next = head;
  head->prev = head;

  struct node* previous = head;
  struct node* current = NULL;
  for (int i = 2; i <= n; i++) {
    current = (struct node*)malloc(sizeof(struct node));
    current->id = i;
    previous->next = current;
    current->prev  = previous;
    previous = current;
  }

  // create loop list
  current->next = head;
  head->prev    = current;

  return head;
}

void deleteList(struct node* head) {
  if (head == NULL) return;

  struct node* current = head;
  do {
    struct node* temp = current;
    current = current->next;
    free(temp);
  } while (current != head);

}

int main() {
  int n, k, m;
  scanf("%d%d%d", &n, &k, &m);

  struct node* head = createList(n);

  // location k
  struct node* current = head;
  for (int i = 1; i < k; i++) {
    current = current->next;
  }

  int count = 1;
  int flag = 0;
  while (flag == 0) {
    if (count == m) {
      // delete node
      struct node* previous = current->prev;
      struct node* temp = current;

	  count = 1;

      if (previous == current) {
        // only one node
        flag = 1;
        printf("%d", current->id);
      } else {
        printf("%d ", current->id);
        current        = current->next;
        previous->next = current;
        current->prev  = previous;
      }

      free(temp);
    } else {
      count++;
      current = current->next;
    }
  }

  printf("\n");
  // deleteList(head);

  return 0;
}

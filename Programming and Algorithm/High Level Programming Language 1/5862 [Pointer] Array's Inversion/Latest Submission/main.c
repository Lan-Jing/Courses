#include <stdio.h>
#include "inv.h"

int main() {
  int n;
  int array[110];
  scanf("%d", &n);

  for (int i = 0; i < n; i++) { scanf("%d", &array[i]); }

  printf("Before inv: ");
  for (int i = 0; i < n - 1; i++) { printf("%d ", array[i]); }
  printf("%d\n", array[n-1]);

  printf("inv\n");
  inv(array, n);

  printf("After inv: ");
  for (int i = 0; i < n - 1; i++) { printf("%d ", array[i]); }
  printf("%d\n", array[n-1]);

  return 0;
}
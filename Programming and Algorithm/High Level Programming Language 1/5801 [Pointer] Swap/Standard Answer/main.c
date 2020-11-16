#include <stdio.h>
#include "swap.h"

int main() {
  int a = 0, b = 0;

  scanf("%d%d", &a, &b);

  printf("Before swap: a=%d, b=%d\n", a, b);

  swap(&a, &b);
  printf("swap a and b\n");

  printf("After swap:  a=%d, b=%d\n", a, b);
  return 0;
}

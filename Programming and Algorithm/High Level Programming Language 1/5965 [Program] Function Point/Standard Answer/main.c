#include <stdio.h>
#include "maxmin.h"

int main() {
  int a, b, c;
  scanf("%d%d", &a, &b);
  scanf("%d", &c);

  printf("a=%d, b=%d\n", a, b);

  if (c == 1) {
    printf("max number: ");
  } else {
    printf("min number: ");
  }

  int (*pf)(int, int) = maxmin(c);
  printf("%d\n", pf(a, b));

  return 0;
}
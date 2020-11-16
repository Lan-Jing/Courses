#include <stdio.h>
#include <string.h>
#include "parseBinary.h"

int main() {
  printf("%d\n", parseBinary("10001"));
  printf("%d\n", parseBinary("10101010101010"));
  printf("%d\n", parseBinary("111111111111"));

  return 0;
}

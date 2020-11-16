#include "inv.h"

void inv(int* array, int size) {
  int temp = 0;
  int m = (size - 1) / 2;

  for (int i = 0, j = size - 1 - i; i <= m; i++) {
    j        = size - 1 - i;
    temp     = array[i];
    array[i] = array[j];
    array[j] = temp;
  }
}

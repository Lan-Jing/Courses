#include <stdio.h>

int array[100100] = {0};

int partition(int lo, int hi) {
  int pivot = array[hi];
  int index = lo - 1;

  for (int j = lo; j < hi; j++) {
    if (array[j] < pivot) {
      if (j != index) {
        index++;
        int temp     = array[index];
        array[index] = array[j];
        array[j]     = temp;
      }
    }
  }

  index++;
  int temp     = array[index];
  array[index] = array[hi];
  array[hi]    = temp;

  return index;
}


void quicksort(int lo, int hi) {
  if (lo < hi) {
    int p = partition(lo, hi);
    quicksort(lo, p - 1);
    quicksort(p + 1, hi);
  }
}

int main() {
  int n;
  scanf("%d", &n);
  for (int i = 0; i < n; i++) { scanf("%d", &array[i]); }

  quicksort(0, n-1);

  for (int i = 0; i < n - 1; i++) {
    printf("%d ", array[i]);
  }
  printf("%d\n", array[n-1]);

  return 0;
}
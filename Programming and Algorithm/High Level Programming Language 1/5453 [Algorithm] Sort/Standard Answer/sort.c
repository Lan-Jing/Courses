#include <stdio.h>

void sort(int array[], int n) {
  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - i - 1; j++) {
      if (array[j] > array[j+1]) {
        int temp  = array[j];
        array[j]  = array[j+1];
        array[j+1] = temp;
      }
    }
  }
}


int main() {
  int array[101];
  int n;

  scanf("%d", &n);
  for (int i = 0; i < n; i++) {
    scanf("%d", &array[i]);
  }

  sort(array, n);

  for (int i = 0; i < n - 1; i++) {
    printf("%d ", array[i]);
  }
  if (n > 0) { printf("%d", array[n - 1]); }
  printf("\n");

  return 0;
}

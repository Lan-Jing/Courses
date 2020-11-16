#include <stdio.h>

int map[17][17] = {0};

int main() {
  int n = 0;
  scanf("%d", &n);

  int maxNum = n * n;
  map[0][n/2] = 1;
  int row = 0, col = n/2;
  for (int num = 2; num <= maxNum; num++) {
    int tempRow = (row - 1 + n) % n;
    int tempCol = (col + 1) % n;

    if (map[tempRow][tempCol] != 0) {
      tempRow = row + 1;
      tempCol = col;
    }

    map[tempRow][tempCol] = num;

    row = tempRow;
    col = tempCol;
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n - 1; j++) {
      printf("%d ", map[i][j]);
    }
    printf("%d\n", map[i][n-1]);
  }
  return 0;
}

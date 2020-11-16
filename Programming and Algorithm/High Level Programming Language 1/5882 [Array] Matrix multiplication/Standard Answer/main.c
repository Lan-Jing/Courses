#include <stdio.h>


void getMatrix(int matrix[][110], int n, int m) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      scanf("%d", &matrix[i][j]);
    }
  }
}

int main() {
  int A[110][110] = {0};
  int B[110][110] = {0};
  int C[110][110] = {0};

  int n, m, r;
  scanf("%d%d%d", &n, &m, &r);

  getMatrix(A, n, m);
  getMatrix(B, m, r);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < r; j++) {
      int ret = 0;
      for (int q = 0; q < m; q++) {
        ret += A[i][q] * B[q][j];
      }
      C[i][j] = ret;
    }
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < r - 1; j++) {
      printf("%d ", C[i][j]);
    }
    printf("%d\n", C[i][r-1]);
  }

  return 0;
}
#include <stdio.h>


void getInput(int A[][110], int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      scanf("%d", &A[i][j]);
    }
  }
}

void add(int A[][110], int B[][110], int C[][110], int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      C[i][j] = A[i][j] + B[i][j];
    }
  }
}

int main() {
  int A[110][110];
  int B[110][110];
  int C[110][110];
  int n;
  scanf("%d", &n);

  getInput(A, n);
  getInput(B, n);

  add(A, B, C, n);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n-1; j++) {
      printf("%d ", C[i][j]);
    }
    printf("%d\n", C[i][n-1]);
  }

  return 0;
}
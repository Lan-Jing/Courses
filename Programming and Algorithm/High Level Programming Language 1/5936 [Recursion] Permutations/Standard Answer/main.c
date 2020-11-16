#include <stdio.h>

int flag[10] = {0};
int ret[10];

void permutation(int n, int deep) {
  if (deep == n) {
    for (int i = 0; i < n; i++) { printf("%d", ret[i]); }
    printf("\n");
  } else {
    for (int i = 0; i < n; i++) {
      if (flag[i] == 0) {
        flag[i] = 1;
        ret[deep] = i + 1;
        permutation(n, deep+1);
        flag[i] = 0;
      }
    }
  }
}

int main() {
  int n;
  scanf("%d", &n);

  permutation(n, 0);

  return 0;
}
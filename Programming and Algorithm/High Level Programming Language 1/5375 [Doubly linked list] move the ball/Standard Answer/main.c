#include <stdio.h>
#define MAXN 500100
int left[MAXN] = {0};
int right[MAXN] = {0};

void link(int x, int y);

int main() {
  int t;
  int n, m;
  int x, y;
  int side;
  int i;
  int ball;

  scanf("%d", &t);

  while (t--) {
    scanf("%d %d", &n, &m);

    for (i = 0; i <= n; i++) {
      link(i, i + 1);
    }

    while (m--) {
      scanf("%d %d %d", &side, &x, &y);

      link(left[x], right[x]);

      if (side == 1) {
        link(left[y], x);
        link(x, y);
      }

      else {
        link(x, right[y]);
        link(y, x);
      }
    }

    for (ball = right[0]; ball != n + 1; ball = right[ball]) {
      printf("%d ", ball);
    }

    printf("\n");
  }
  return 0;
}

void link(int x, int y) {
  right[x] = y;
  left[y] = x;
}
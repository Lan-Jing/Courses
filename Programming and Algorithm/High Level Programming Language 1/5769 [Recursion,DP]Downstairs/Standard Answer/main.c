#include <stdio.h>

int result = 0;

int stairs(int stairNum, int maxStep) {
  if (stairNum == 0) {
    result++;
  } else {
    for (int step = 1; step <= maxStep; step++) {
      if (stairNum < step) { break; }
      stairs(stairNum - step, maxStep);
    }
  }
}


int main() {
  int h = 0;
  int s = 0;
  scanf("%d%d", &h, &s);

  stairs(h, s);

  printf("%d\n", result);

  return 0;
}
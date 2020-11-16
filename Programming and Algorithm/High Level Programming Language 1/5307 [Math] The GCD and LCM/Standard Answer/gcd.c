#include <stdio.h>

int gcd(int num1, int num2) {
  if (num1 < num2) {
    int temp = num1;
    num1 = num2;
    num2 = temp;
  }

  if (num2 == 0) { return num1; }

  return gcd(num2, num1 % num2);
}

int lcm(int num1, int num2) {
  return num1  / gcd(num1, num2) * num2;
}

int main() {
  int t;
  scanf("%d", &t);

  int num1, num2;
  for (; t > 0; t--) {
    scanf("%d%d", &num1, &num2);

    if (num1 <= 0 || num2 <= 0) {
      printf("invalid\n");
    } else {
      printf("%d %d\n", gcd(num1, num2), lcm(num1, num2));
    }
  }

  return 0;
}
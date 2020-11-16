#include <stdio.h>

void move(int n, int a, int b) {
    /* n piece , from a to b */
    if (n == 1) printf("%d%d\n", a, b);
    else {
        int leave = 6 - a - b;
        move(n - 1, a, leave);
        move(1, a, b);
        move(n - 1, leave, b);
    }
}

int main() {
    int n = 0;
    scanf("%d", &n);

    move(n, 1, 3);
    return 0;
}
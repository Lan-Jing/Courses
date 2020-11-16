#include <stdio.h>
int get_len(int x) {
    int num = 1;
    while (x != 1) {
        x = x % 2 ? 3 * x + 1 : (x >> 1);
        num++;
    }
    return num;
}
int main() {
    int i, start, end, max = 0, tmp;
    scanf("%d%d", &start, &end);
    for (i = start; i <= end; i++) {
        tmp = get_len(i);
        max = tmp > max ? tmp : max;
    }
    printf("%d\n", max);
    return 0;
}

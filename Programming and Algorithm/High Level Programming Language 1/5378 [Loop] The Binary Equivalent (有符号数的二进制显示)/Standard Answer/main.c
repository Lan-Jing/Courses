#include <stdio.h>

char* toBase2(int n, char *save) {
    if (n < 0) n = 65536 + n;
    int i = 0;
    for (i = 15; i >= 0; i--) {
        if (n > 0) {
            save[i] = (n % 2) + '0';
            n /= 2;
        } else {
            save[i] = '0';
        }
    }
    return save;
}

int main() {
    int n = 0;
    while (scanf("%d", &n) != EOF) {
        char save[20] = {0};
        printf("%s\n", toBase2(n, save));
    }
    return 0;
}
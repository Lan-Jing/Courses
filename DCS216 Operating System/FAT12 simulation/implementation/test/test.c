#include <stdio.h>
#include <conio.h>

int main() {
    char ch;
    while(1) {
        ch = getch();
        printf("%c %d\n", ch, (int)ch);
    }
    return 0;
}
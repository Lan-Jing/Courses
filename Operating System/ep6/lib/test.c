int start() {
    char ch, str[40];
    ch = getch();
    putchar(ch);
    newline();
    ch = getchar();
    putchar(ch);
    newline();
    gets(str);
    puts(str);
    getchar();
    return 0;
}
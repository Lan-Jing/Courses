int strlen(char *str) {
    int l = 0;
    for(;str[l];l++);
    return l;
}
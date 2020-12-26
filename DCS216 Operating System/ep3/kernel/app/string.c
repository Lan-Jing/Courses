int countA(char *message, int length) {
    int i = 0, ans = 0;
    for(;i < length;i++)
        if(message[i] == 'a' || message[i] == 'A')
            ans++;
    return ans;
}
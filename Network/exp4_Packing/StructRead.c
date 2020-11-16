#include <stdio.h>
#include <string.h>
#include <time.h>
#include "struct.h"

int main() {
    FILE* fp = fopen("Persons.stru", "rb");
    if(fp == NULL) {
        printf("Failed to open file\n");
        return 0;
    }

    struct Person now;
    while(fscanf(fp, "%s%d%s%d%d", 
                 now.username, &now.level, now.email, &now.sendtime, &now.regtime)
          != EOF) {
        printf("UserName: %s\n", now.username);
        printf("Income level: %d\n", now.level);
        printf("Email address: %s\n", now.email);
        printf("Send time: %s", ctime(&now.sendtime));
        printf("Register time: %s\n", ctime(&now.regtime));
    }
    fclose(fp);
    return 0;
}

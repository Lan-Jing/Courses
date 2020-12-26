#include <stdio.h>
#include <string.h>
#include <time.h>
#include "struct.h"

int main() {
    FILE* fp = fopen("Persons.stru", "wb");
    if(fp == NULL) {
        printf("Failed to open file\n");
        return 0;
    }

    struct Person now;
    while(1) {
        // set basic information;
        printf("Username: ");
        scanf("%s", now.username);
        if(!strcmp(now.username, "exit")) break;
        printf("Income level: ");
        scanf("%d", &now.level);
        printf("Email address: ");
        scanf("%s", now.email);

        // set the current time;
        time_t nowTime = time(0);
        now.sendtime  = now.regtime = nowTime;
        printf("\n");

        // write in file;
        fprintf(fp, "%s\n%d\n%s\n%d\n%d\n", now.username, now.level, now.email, now.sendtime, now.regtime);
    }

    fclose(fp);
    return 0;
}
#pragma once

#include <string.h>

#define FILE_NAME_LEN 300

void get_fileName(char* fileName) {
    int start = 0, l = strlen(fileName);
    for (int i = 0; i < l; i++) {
        if (fileName[i] == '\\' || fileName[i] == '/')
            start = i;
    }
    char newFileName[FILE_NAME_LEN];
    memcpy(newFileName, fileName, l + 1);
    memset(fileName, 0, l + 1);
    memcpy(fileName, newFileName + start, l - start + 1);
}
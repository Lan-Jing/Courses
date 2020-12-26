#ifndef FILESTRUCT_H
#define FILESTRUCT_H

#include <string.h>

#define FILE_NAME_LEN 300
#pragma pack(1)

struct FileStruct {
    char fileName[FILE_NAME_LEN];
    long long int fileSize;
};

void get_fileName(char *fileName) {
    int start = 0, l = strlen(fileName);
    for(int i = 0;i < l;i++) {
        if(fileName[i] == '\\' || fileName[i] == '/')
            start = i;
    }
    char newFileName[FILE_NAME_LEN];
    memcpy(newFileName, fileName, l+1);
    memset(fileName, 0, l+1);
    memcpy(fileName, newFileName+start, l-start+1);
}

int get_fileSize(FILE *fp) {
    unsigned int curPos = ftell(fp);
    int fileSize = 0;
    fseek(fp, 0, SEEK_END);
    fileSize = ftell(fp);
    fseek(fp, curPos, SEEK_SET);

    return fileSize;
}

void strcat_noext(char *fileName, char *newPart) {
    char newFileName[FILE_NAME_LEN];
    int ptr = 0, flag = 0;
    for(int i = 0;i < strlen(fileName);i++) {
        if(fileName[i] == '.') {
            flag = i;
            break;
        }
        newFileName[ptr++] = fileName[i];
    }
    for(int i = 0;i < strlen(newPart);i++) {
        newFileName[ptr++] = newPart[i];
    }
    for(int i = flag;i < strlen(fileName);i++) {
        newFileName[ptr++] = fileName[i];
    }
    newFileName[ptr] = '\0';
    memcpy(fileName, newFileName, strlen(newFileName)+1);
}

#endif
#include <iostream>
using namespace std;
#include <stdio.h>
#include <string>
#include "FileStruct.h"

#include <map>
map<string, int> Map;

#define bufSize 1000

int main() {
    char path[FILE_NAME_LEN];
    struct FileStruct tar, src;
    printf("Enter the path of the target files: ");
    scanf("%s", path);
    if(path[strlen(path)] != '\\')
        strcat(path, "\\"); 

    memset(&src, 0, sizeof(struct FileStruct));
    printf("Enter the path of the packed file: ");
    scanf("%s", src.fileName);

    FILE* srcFp = fopen(src.fileName, "rb");
    if(srcFp == NULL) {
        printf("Failed to open file %s\n", src.fileName);
        return 0;
    }
    
    while(1) {
        memset(&tar, 0, sizeof(tar));
        // fscanf(srcFp, "%s", tar.fileName);
        // if(!strlen(tar.fileName)) break;
        // fscanf(srcFp, "%d", &tar.fileSize);
        // fgetc(srcFp);
        // this line to unpack others' file.
        if(fread(&tar, sizeof(char), sizeof(struct FileStruct), srcFp) < sizeof(struct FileStruct))
            break;
        if(Map.count(tar.fileName)) {
            char tmpStr[10];
            sprintf(tmpStr, "(%d)", Map[tar.fileName]);
            Map[tar.fileName]++;
            strcat_noext(tar.fileName, tmpStr);
        }
        Map[tar.fileName]++;

        char tmpStr[2 * FILE_NAME_LEN];
        memcpy(tmpStr, path, strlen(path)+1);
        strcat(tmpStr, tar.fileName);
        memcpy(tar.fileName, tmpStr, strlen(tmpStr)+1);
        printf("Unpacking %s with size %d\n", tar.fileName, tar.fileSize);

        FILE* tarFp = fopen(tar.fileName, "wb");
        if(tarFp == NULL) {
            printf("Failed to open file %s\n", tar.fileName);
            break;
        }

        char tmp;
        for(int i = 0;i < tar.fileSize;i++) {
            tmp = fgetc(srcFp);
            fwrite(&tmp, sizeof(char), 1, tarFp);
        }
        fclose(tarFp);
    }

    fclose(srcFp);
    return 0;
}
#include <stdio.h>
#include "FileStruct.h"

#define bufSize 1000

int main() {
    struct FileStruct src, tar;
    printf("Enter the target path of the packed file: ");
    scanf("%s", tar.fileName);

    FILE* fp = fopen(tar.fileName, "wb");
    if(fp == NULL) {
        printf("Failed to open file %s\n", tar.fileName);
        return 0;
    }

    int counter = 0, len = 0;
    char buffer[bufSize];
    while(1) {
        memset(&src, 0, sizeof(struct FileStruct));
        printf("Enter path of the file No.%d you want to pack: ", ++counter);
        scanf("%s", src.fileName);
        if(!strcmp(src.fileName, "exit")) break;

        FILE* srcFp = fopen(src.fileName, "rb");
        if(srcFp == NULL) {
            printf("Failed to open file %s\n", src.fileName);
            break;
        }
        get_fileName(src.fileName);
        src.fileSize = get_fileSize(srcFp);
        // fprintf(fp, "%s\n%d\n", src.fileName, src.fileSize);   
        // this line for others to unpack
        fwrite(&src, sizeof(char), sizeof(struct FileStruct), fp);     
        
        int len = 0;
        while(len = fread(buffer, sizeof(char), bufSize, srcFp)) {
            fwrite(buffer, sizeof(char), len, fp);
        }
        fclose(srcFp);
    }

    fclose(fp);
    return 0;
}
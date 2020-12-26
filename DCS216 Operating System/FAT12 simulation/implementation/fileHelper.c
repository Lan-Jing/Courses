#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "..\headers\fileSys.h"

/*-------------------------------------------------------------*/

void init() {
    dirPtr = 19;
    strcpy(curPath, ""); // set initial path and pointer; 
}

int Min(int a, int b) {
    return a < b ? a : b;
}

int Max(int a, int b) {
    return a < b ? b : a;
}

int decode_path(char *path, char *nowName) {
    static int sp = 0;
    int np = 0;
    if(nowName[0] == '\0')
        sp = 0; // reset the pointer;
    
    while(path[sp] != '\0' && path[sp] != '\\' && path[sp] != '/') 
        nowName[np++] = path[sp++];
    nowName[np] = '\0';
    // construct the next name;
    if(path[sp] == '\\' || path[sp] == '/') sp++; 
    // if the path continues;

    if(!file_checkName(nowName, 1)) {
        printf("File name: %s is invalid\n", nowName);
        return 0;
    } else 
        return 1;
}

void decode_command(char *command, char *nowParameter) {
    static int cp = 0;
    if(nowParameter[0] == '\0') 
        cp = 0;
    
    int pp = 0, preFlag = 0;
    while(command[cp] != '\0') {
        if(command[cp] == ' ') {
            if(preFlag) 
                break;
            else 
                cp++; // ignore spaces before parameters;
        } else {
            preFlag = 1;
            nowParameter[pp++] = command[cp++];
        }
    }
    nowParameter[pp] = '\0'; // get next parameter;
    if(command[cp] == ' ') cp++; // skip the space and wait for the next call;
}

void get_pre_path(char *path) {
    int l = strlen(path), i;
    for(i = l-1;i >= 0 && path[i] != '\\';i--);
    path[i < 0 ? 0 : i] = '\0';
}

void get_post_path(char *requestPath, char *postPath) {
    char tmp[256]; tmp[0] = '.';
    do {
        decode_path(requestPath, tmp);
        if(strlen(tmp)) strcat(postPath, "\\");
        strcat(postPath, tmp);
    }while(tmp[0]);
} // iteratively get the rest of the string

struct findNode dfs_find(int FATp, char *requestPath, char *nowPath, char *nowName, int mode) {
    int ifRoot = nowPath[0] == '\0' ? 1 : 0;
    // freeNode records possible position of the new file
    struct findNode findRes, freeNode;
    // firstFAT place the head sector of the current directory, only used for ".." to link back
    freeNode.firstFAT = freeNode.FAT = freeNode.firstClus = -1;
    
    struct entry curEntry; curEntry.DIR_Name[0] = 0;
    int nowSec = FATp, offset = 0;
    findRes.firstFAT = freeNode.firstFAT = nowSec;
    while(file_loadEntry(&curEntry, nowSec, offset, ifRoot), curEntry.DIR_Name[0]) {
        // record the first available location for a new file, return it if target is not found;
        if(freeNode.FAT == -1) {
            // a deleted file;
            if(curEntry.DIR_Name[0] == 0xE5)
                freeNode.FAT = nowSec, freeNode.offset = offset, freeNode.pathType = ifRoot;
            // a full sector;
            if(offset == mbr.BPB_BytsPerSec / entrySize - 1 && 
                ((ifRoot && FAT12_get_next(nowSec, ifRoot) == 33) || 
                (!ifRoot && FAT12_get_next(nowSec, ifRoot) == 0xFFF)))
                freeNode.FAT = nowSec, freeNode.offset = -1, freeNode.pathType = ifRoot;
        }

        if(!file_strcmp(curEntry.DIR_Name, nowName, attr_check(curEntry.DIR_Attr, 4))) {
            file_moveEntry(&nowSec, &offset, ifRoot);
            continue;
        } // fail to match name;

        if(strcmp(nowName, ".") && strcmp(nowName, "..")) {
            if(nowPath[0]) strcat(nowPath, "\\");
            strcat(nowPath, nowName);
        } else if(!strcmp(nowName, "..")) {
            get_pre_path(nowPath);
        } // maintain current dir path;

        if(!decode_path(requestPath, nowName) || // invalid file name;
          (!attr_check(curEntry.DIR_Attr, 4) && (nowName[0] != '\0' || mode))) 
            continue;
        // match a file but not in the last lavel, or running a cd command;

        if(nowName[0] == '\0') {
            findRes.FAT = nowSec;
            findRes.offset = offset;
            findRes.firstClus = curEntry.DIR_FstClus ? curEntry.DIR_FstClus : 19;
            findRes.pathType = ifRoot;
            findRes.fileType = attr_check(curEntry.DIR_Attr, 4);
            memset(findRes.postPath, 0, sizeof(findRes.postPath));
            // the path ends, location found;
            return findRes;
        } else {
            return dfs_find(curEntry.DIR_FstClus ? curEntry.DIR_FstClus : 19, 
                            requestPath, nowPath, nowName, mode);
        } // enter the next level, only for directory;
    }
    // to the end of the directory;
    if(freeNode.FAT == -1) 
        freeNode.FAT = nowSec, freeNode.offset = offset, freeNode.pathType = ifRoot;
    memset(freeNode.postPath, 0, sizeof(freeNode.postPath));
    strcat(freeNode.postPath, nowName);
    get_post_path(requestPath, freeNode.postPath);
    memcpy(&findRes, &freeNode, sizeof(struct findNode));
    findRes.fileType = file_checkName(findRes.postPath, 0)-1;

    return findRes;
}

void print_findRes(struct findNode res) {
    printf("FAT:%d offset:%d\n", res.FAT, res.offset);
    printf("firstClus:%d pathType:%d fileType:%d\n", res.firstClus, res.pathType, res.fileType);
    printf("postPath:%s\n", res.postPath);
}

void dfs_display_tree(int FATp, int ifRoot, int k, int curLayer) {
    if(curLayer > k) return ;
    
    struct entry curEntry; curEntry.DIR_Name[0] = 0;
    int nowSec = FATp, offset = 0;
    while(file_loadEntry(&curEntry, nowSec, offset, ifRoot), curEntry.DIR_Name[0]) {
        if(curEntry.DIR_Name[0] == '.' || curEntry.DIR_Name[0] == 0xE5) {
            file_moveEntry(&nowSec, &offset, ifRoot);
            continue;
        } // special or deleted files;

        if(!attr_check(curEntry.DIR_Attr, 1)) {
            for(int i = 1;i < curLayer;i++)
                printf("    "); // indent showing file level;
            printf("|--");
            display_fileName(curEntry.DIR_Name, (attr_check(curEntry.DIR_Attr, 4)));
            printf("\n");
        } // a visible file to display;

        if(attr_check(curEntry.DIR_Attr, 4)) 
            dfs_display_tree(curEntry.DIR_FstClus, 0, k, curLayer + 1);
        // a dir file invoking recursion;

        file_moveEntry(&nowSec, &offset, ifRoot);
    }
}
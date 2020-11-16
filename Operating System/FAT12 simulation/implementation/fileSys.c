#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

#include "..\headers\filesys.h"

extern struct MBR mbr; // The only MBR record read from the system image;

char image[512 * 2880]; // image and its location;
char *imagePath;

char curPath[MaxCommandSize]; // pointer to the current directory;
int  dirPtr;

const char *commandList[] = {"help",        // output command descriptions;
                             "pwd",         // return the current path;
                             "find",        // return FAT index according to the given path string;
                             "cd",          // find path then enter;
                             "tree",        // display dir tree, k layers max;
                             "dir",         // display the current directory;
                             "copy",        // copy a file;
                             "touch",       // make a new, empty file;
                             "mkdir",       // make a new, empty directory;
                             "md",          // shortcut of mkdir;
                             "rm",          // remove a file;
                             "rmdir",       // remove an empty directory;
                             "rd",          // shortcut of rmdir;
                             "rename",      // rename the given file/dir;
                             "open",        // open and display a given text file
                             "edit",        // edit a given text file;
                             "cls"          // empty the screen;
                            };         

const char *description[] = {"Output help information",
                             "Output the current path",
                             "try to find the given file/dir",
                             "Find then enter the given path",
                             "Display the k-layer dir tree, k is set 3 by default",
                             "Display the current directory",
                             "Copy an existing file to the current directory",
                             "Make a new, empty file",
                             "Make a new, empty directory",
                             "Shortcut of mkdir",
                             "Remove a file",
                             "Remove an empty directory",
                             "Shortcut of rmdir",
                             "Rename the given file/dir",
                             "Open and display a given text file",
                             "Edit a given text file",
                             "empty the screen"
                            };
/*-------------------------------------------------------------------*/

int image_open(char *filePath) {
    init(); // set default parameter;

    FILE *imageFile = fopen(filePath, "rb");
    if(imageFile == NULL) {
        printf("Failed to open OS image\n");
        return -1;
    }
    int readCnt = fread(image, sizeof(char), sizeof(image), imageFile); 
    fclose(imageFile);
    if(readCnt < sizeof(image)) {
        printf("Failed to load OS image\n");
        return -1;
    }

    MBR_get_MBR(image);         // load MBR;
    MBR_check(&mbr);            // check MBR;
    FAT12_get_FAT(image, 0);    // load FAT;

    return 1;
}

int image_close(char *filePath) {
    FILE *imageFile = fopen(filePath, "wb");
    if(imageFile == NULL) {
        printf("Failed to open OS image\n");
        return -1;
    }

    FAT12_get_FAT(image, 1);    // write back FAT sectors;

    int writeCnt = fwrite(image, sizeof(char), sizeof(image), imageFile);
    fclose(imageFile);
    if(writeCnt < sizeof(image)) {
        printf("Failed to write back OS image\n");
        return -1;
    }
    return 1;
}

void help() {
    printf("FAT12 file system, writen by Jing Lan, Sun Yat-sen University. \n");
    printf("Please use 'exit' command instead of Ctrl+c to quit the programm.\n");

    printf("Descriptions on supported commands:\n");
    for(int i = 0;i < CommandSupported;i++)
        printf("%10s : %s\n", commandList[i], description[i]);
    printf("Run 'help' command to display again.\n");
}

void pwd() {
    printf("%s\n", curPath);
} // display the current path string;

void cd(char *path) {
    find(path, 1);
} // Enter the target directory by adapting dirPtr and curPath;

struct findNode find(char *requestPath, int mode) {
    struct findNode findRes; findRes.FAT = findRes.firstClus = -1;

    char nowName[12]; nowName [0] = '\0';
    if(!decode_path(requestPath, nowName)) // get the first name;
        return findRes; // invalid name;
    if(nowName[0] == '\0') {
        findRes.FAT = dirPtr, findRes.offset = 0;
        findRes.pathType = curPath[0] == '\0', findRes.firstClus = dirPtr, findRes.fileType = 1;
        return findRes;
    } // current directory;

    char nowPath[MaxCommandSize]; // maintain a new path for cd command;
    memcpy(nowPath, curPath, strlen(curPath)+1);

    findRes = dfs_find(dirPtr, requestPath, nowPath, nowName, mode);
    // print_findRes(findRes);
    if(findRes.firstClus != -1) {
        if(mode) {
            dirPtr = findRes.firstClus;
            memcpy(curPath, nowPath, strlen(nowPath)+1);
        } // enter the given path;
        printf("File/Path: %s found\n", requestPath);
        return findRes;
    } // target found;

    if(findRes.fileType < 0)
        printf("Invalid path: %s\n", findRes.postPath);
    else 
        printf("File/Path: %s not found\n", findRes.postPath);
    return findRes;
} // return FAT index, change curPath if mode == 1


void dir() {
    char *dirSec = (char *)malloc(mbr.BPB_BytsPerSec);
    int ifRoot = curPath[0] == '\0' ? 1 : 0;
    for(int nowSec = dirPtr;ifRoot ? nowSec < 33 : nowSec != 0xFFF;nowSec = FAT12_get_next(nowSec, ifRoot)) {
        memcpy(dirSec, image + FAT12_get_secAdd(nowSec, 0, ifRoot), mbr.BPB_BytsPerSec);
        display_dirInfo(dirSec, mbr.BPB_BytsPerSec / entrySize);        
    }
    free(dirSec);

    struct dirNode dirInfo = dir_count_file(dirPtr, ifRoot);
    printf("%9d file(s)  %10d bytes\n", dirInfo.fileCount, dirInfo.memUsed);
}

void tree(char *path, int k) {
    struct findNode findRes = find(path, 0);   // locate the given directory;
    if(findRes.firstClus == -1 || findRes.fileType < 1) {
        if(findRes.fileType == 0) {
            printf("Enter name for a directory to display the dir tree\n");
        }
        return ;
    }
    
    dfs_display_tree(findRes.firstClus, path[0] == '\0', k, 1);
}

void touch(char *filePath) {
    struct findNode fileLoc = find(filePath, 0);
    if(fileLoc.firstClus != -1) {
        struct entry fileEntry;
        memcpy(&fileEntry, image + FAT12_get_secAdd(fileLoc.FAT, fileLoc.offset, fileLoc.pathType), entrySize);
        printf("File %s already exists\n", filePath);
        return ;
    }
    if(fileLoc.fileType < 0) {
        printf("%s is not a valid file name\n", fileLoc.postPath);
        return ;
    }
    if(fileLoc.offset == -1) {
        if(fileLoc.pathType) {
            printf("Running out of root directory space\n");
            return ;
        }
        FAT12_push_back_new_sector(fileLoc.FAT);
        // count function returns the last sector index if it's full;
        fileLoc.FAT = FAT12_get_next(fileLoc.FAT, 0), fileLoc.offset = 0;
        // enter the new sector;
    } // free sector not found: allocate a new one;

    struct entry *curEntry = (struct entry *)(image + FAT12_get_secAdd(fileLoc.FAT, fileLoc.offset, fileLoc.pathType));
    file_createFile(curEntry, fileLoc.postPath, fileLoc.fileType, fileLoc.firstFAT);
}

void copy(char *oldFilePath, char *newFilePath) {
    // find and load the source file;
    struct findNode sourceLoc = find(oldFilePath, 0);
    struct entry *sourceFile, *newFile;
    if(sourceLoc.firstClus == -1) {
        printf("Source file not found\n");
        return ;
    } else {
        // if the file is a directory;
        if(sourceLoc.fileType) {
            printf("PLease specify name for a file\n");
            return ;
        }
        sourceFile = (struct entry *)(image + FAT12_get_secAdd(sourceLoc.FAT, sourceLoc.offset, sourceLoc.pathType));
    }
    // load file content into memory;
    char oldFileContent[512 * 2880];
    file_loadFile(sourceFile, oldFileContent);

    // copy the source file to the targeted one;
    struct findNode targetLoc = find(newFilePath, 0);
    if(targetLoc.firstClus == -1) { // new file to be created;
        if(!file_checkName(targetLoc.postPath, 0)) {
            printf("%s is not a valid file name\n", targetLoc.postPath);
            return ;
        }
        if(targetLoc.offset == -1) { // a full sector;
            if(targetLoc.pathType) {
                printf("Running out of root directory space\n");
                return ;
            }
            // get a new entry sector;
            FAT12_push_back_new_sector(targetLoc.FAT);
            targetLoc.FAT = FAT12_get_next(targetLoc.FAT, 0), targetLoc.offset = 0;
        }
        // create and fill content to the new file;
        newFile = (struct entry *)(image + FAT12_get_secAdd(targetLoc.FAT, targetLoc.offset, targetLoc.pathType));
        file_createFile(newFile, targetLoc.postPath, 0, targetLoc.firstFAT);
        file_writeFile(0, newFile, oldFileContent, sourceFile->DIR_FileSize);
    } else {
        printf("Copy to an existing file, overwrite or append:[o\\a]:");
        char para = getch();
        printf("\n");
        if(para != 'o' && para != 'a') {
            printf("Wrong parameter, try the command again\n");
            return ;
        } else {
            newFile = (struct entry *)(image + FAT12_get_secAdd(targetLoc.FAT, targetLoc.offset, targetLoc.pathType));
            file_writeFile(para == 'o', newFile, oldFileContent, sourceFile->DIR_FileSize);
        }
    }
}

void rm(char *filePath) {
    struct findNode fileLoc = find(filePath, 0);
    struct entry *fileEntry = (struct entry *)malloc(entrySize); 

    if(fileLoc.firstClus == -1) {
        printf("File %s not found\n", filePath);
        return ;
    } else {
        fileEntry = (struct entry *)(image + FAT12_get_secAdd(fileLoc.FAT, fileLoc.offset, fileLoc.pathType));

        if(attr_check(fileEntry->DIR_Attr, 0)) {
            printf("Unable to write a read-only file\n");
            return ;
        } // check if the file is read-only;
        if(fileLoc.fileType) { 
            struct dirNode dirInfo = dir_count_file(fileEntry->DIR_FstClus, 0);
            if(dirInfo.totCount) {
                printf("Directory ");
                display_fileName(fileEntry->DIR_Name, fileLoc.fileType);
                printf("is not empty\n");
                return ;
            }
        } // directory: check if it is empty;
    }
    
    // only modify the first character then wait for a replacement;
    fileEntry->DIR_Name[0] = 0xE5;
    // set the Achieve Flag in the attribute byte;
    attr_set(&fileEntry->DIR_Attr, 5);
}

void rn(char *oldFilePath, char *newFileName) {
    struct findNode fileLoc = find(oldFilePath, 0);
    if(fileLoc.firstClus == -1) {
        printf("File %s not found\n", oldFilePath);
        return ;
    } // find the targeted file;
    if(!file_checkName(newFileName, 0)) {
        printf("%s is not a valid name\n", newFileName);
        return ;
    } // validate the new name;

    struct entry *fileEntry = (struct entry *)(image + FAT12_get_secAdd(fileLoc.FAT, fileLoc.offset, fileLoc.pathType));
    // set file name;
    file_setName(fileEntry->DIR_Name, newFileName);
    // set last modification time;
    file_setDateAndTime(&fileEntry->DIR_WrtDate, &fileEntry->DIR_WrtTime);
    // set the Achieve Flag;
    attr_set(&fileEntry->DIR_Attr, 5);
}

void open(char *filePath) {
    struct findNode findRes = find(filePath, 0);
    if(findRes.firstClus == -1) {
        printf("File %s not found\n", filePath);
        return ;
    } // find the file;
    if(findRes.fileType) {
        printf("Please enter name for a text file\n");
        return ;
    } // check file type;
    display_fileContent(findRes.FAT, findRes.offset, findRes.pathType);
}

void edit(char *filePath) {
    struct findNode findRes = find(filePath, 0);
    if(findRes.firstClus == -1) {
        printf("File %s not found\n", filePath);
        return ;
    } // find the file;
    if(findRes.fileType) {
        printf("Please enter name for a text file\n");
        return ;
    } // check file type;

    char buffer[MaxCommandSize]; 
    memset(buffer, 0, sizeof(buffer));
    gets(buffer);

    struct entry *curFile = (struct entry *)(image + FAT12_get_secAdd(findRes.FAT, findRes.offset, findRes.pathType));
    file_writeFile(1, curFile, buffer, strlen(buffer)+1);
}

void cmd(char *commandLine) {
    char command[MaxCommandSize]; 
    command[0] = '\0';
    decode_command(commandLine, command);
    
    int   commandInd = -1;
    for(int i = 0;i < CommandSupported;i++) {
        if(!strcmp(command, commandList[i])) {
            commandInd = i;
            break;
        }
    } // get id of the wanted command;
    switch(commandInd) {
        case 0 : {
            help();
            break;
        }
        case 1 : {
            pwd(); 
            break;
        }
        case 2 : {
            decode_command(commandLine, command);
            struct findNode res = find(command, 0);
            break;
        }
        case 3 : {
            decode_command(commandLine, command);
            cd(command);
            break;
        }
        case 4 : {
            decode_command(commandLine, command);
            tree(command, maxLayer);
            break;
        }
        case 5 : {
            dir();
            break;
        }
        case 6 : {
            char oldFilePath[MaxCommandSize], newFilePath[MaxCommandSize]; 
            oldFilePath[0] = newFilePath[0] = '.';
            decode_command(commandLine, oldFilePath);
            decode_command(commandLine, newFilePath); 
            copy(oldFilePath, newFilePath);
            break;
        }
        case 7 : 
        case 8 : 
        case 9 : {
            decode_command(commandLine, command);
            touch(command);
            break;
        }
        case 10 : 
        case 11 :
        case 12 : {
            decode_command(commandLine, command);
            rm(command);
            break;
        }
        case 13 : {
            char oldFilePath[MaxCommandSize], newFileName[MaxCommandSize]; 
            oldFilePath[0] = newFileName[0] = '.';

            decode_command(commandLine, oldFilePath);
            decode_command(commandLine, newFileName);
            rn(oldFilePath, newFileName);
            break;
        }
        case 14 : {
            decode_command(commandLine, command);
            open(command);
            break;
        }
        case 15 : {
            decode_command(commandLine, command);
            edit(command);
            break;
        }
        case 16 : {
            system("cls");
            break;
        }
        default : {
            printf("Invalid Command\n");
            break;
        }
    }
    return ;
}
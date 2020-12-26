#ifndef FILEHELPER_H
#define FILEHELPER_H

// set default parameters after file is opened;
void init();

// return the smaller integer;
int  Min(int a, int b);

// return the bigger integer;
int  Max(int a, int b);

// split path string into seperate filenames;
int  decode_path(char *path, char *nowName);

// split the command by space;
void decode_command(char *command, char *nowParameter); 

// back to the previous level;
void get_pre_path(char *path);

// return unmatched path;
void get_post_path(char *requestPath, char *postPath);

// return type indicating search result;
struct findNode {
    int FAT, offset;                             // its location in the image;
    int firstFAT, firstClus, pathType, fileType; // file information
    char postPath[256];                          // unmatched path name;
}; 
// sub-routine that enable recursion;
struct findNode dfs_find(int FATp, 
                         char *requestPath, char *nowPath, char *nowName, 
                         int mode); 

// sub-routine that preforms recursive display;
void dfs_display_tree(int FATp, int ifRoot, int k, int curLayer); 

void print_findRes(struct findNode res);

#endif
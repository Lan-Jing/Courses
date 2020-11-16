#ifndef FILESYS_H
#define FILESYS_H 

#include "..\headers\dir.h"
#include "..\headers\MBR.h"
#include "..\headers\FAT.h"
#include "..\headers\fileHelper.h"

#define MaxCommandSize 256
#define CommandSupported 17
#define maxLayer 3

// array that store the whole file system;
extern char image[512 * 2880];

// image path string;
extern char *imagePath;

// pointer pointed to the current directory;
extern int dirPtr;

// string containing the current path;
extern char curPath[MaxCommandSize];

// supported command strings;
extern const char *commandList[];

// description of the supported commands;
extern const char *description[];

/*-----------------------------------------------*/

// open and initilize the disk image;
int  image_open(char *filePath);

// write back image into the file;
int  image_close(char *filePath);

// output command desrciptions;
void help();

// return FAT index according to the given path string;
struct findNode find(char *requestPath, int mode); 

// return the current path;
void pwd(); 

// find path then enter;
void cd(char *path); 

// display the current directory;
void dir(); 

// display dir tree, k layers max;
void tree(char *path, int k); 

// tranlate the given command then run corresponding functions;
void cmd(char *commandLine); 

// create a new file/dir;
void touch(char *fileName);

// copy a file;
void copy(char *oldFileName, char *newFileName);

// remove a file/empty dir;
void rm(char *fileName);

// rename a given file/dir;
void rn(char *oldFileName, char *newFileName);

// open and display a given text file;
void open(char *filePath);

#endif
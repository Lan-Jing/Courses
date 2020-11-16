#ifndef ROOTDIR_H
#define ROOTDIR_H

#pragma pack(1)

#define entrySize 32
struct entry {
    unsigned char DIR_Name[11]; // file name with a 8-byte name and a 3 -byte extension name;
    char DIR_Attr; // file attribution;
    char Reserve[10]; // Reserved bytes that can be utilized later;
    unsigned short DIR_WrtTime; // Last Write-in time;
    unsigned short DIR_WrtDate; // Last Write-in date;
    unsigned short DIR_FstClus; // cluster index that this file begins;
    unsigned int  DIR_FileSize; // size of the file;
};

#define fbListSize 16
// list of forbidded characters in files name;
extern char forbiddedList[];

// for function count_file_and_memory;
struct dirNode {
    int fileCount, memUsed, totCount;  
    // the first available location for a new file/dir; 
};

/*-----------------------------------------------*/

// check attribute of the given file;
int  attr_check(char attr, int loc);

// set attribute of the given file;
void attr_set(char *attr, int loc);

// display file names and extensions;
void display_fileName(char *fileName, int dir);

// display information of a single root entry;
void display_entryInfo(struct entry *item);

// display the whole directory;
void display_dirInfo(char *sector, int cnt);

// display the content of a file in text format;
void display_fileContent(int startSec, int offset, int ifRoot);

// fill zeros into a sector;
void file_freeSec(int FAT12Index);

// write data into the given file;
int  file_writeFile(int mode, struct entry *fileEntry, char *data, int dataSize);

// load a given file to memory as char string;
void file_loadFile(struct entry *file, char *data);

// load one file entry at a time;
void file_loadEntry(struct entry *enp, int startSec, int startOffset, int ifRoot);

// seperate part used for pointer movement; 
void file_moveEntry(int *sec, int *offset, int ifRoot);

// write date information;
void file_setDateAndTime(unsigned short *DIR_WrtDate, unsigned short *DIR_WrtTime);

// transfer date digits into readable format;
void file_getDate(unsigned short DIR_WrtDate);

// transfer time digits into readable format;
void file_getTime(unsigned short DIR_WrtTime);

// check if the given file name is valid;
int  file_checkName(char *fileName, int emptyAllowed);

// transfer the name format then fill the name sector;
void file_setName(char *nameSec, char *newName);

// match file or dir name;
int  file_strcmp(char *target, char *source, int dir);

// fill file info;
void file_createFile(struct entry *curEntry, char *fileName, int dir, int FATp);

// count number of files and memory used in the current directory;
struct dirNode dir_count_file(int FATp, int ifRoot);

// sub-routine that only search the current directory for a given file;
struct dirNode dir_find(char *requestName);

#endif 
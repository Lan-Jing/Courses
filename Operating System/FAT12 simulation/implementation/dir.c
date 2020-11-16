#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "..\headers\fileSys.h"

char forbiddedList[16] = {'"', '*', '+', ',', '.', '/', ':', ';', '<', '=' ,'>', '?', '[', '\\', ']', '|'};

/*--------------------------------------------------------*/

void display_fileName(char *fileName, int dir) {
    for(int i = 0;i < 11;i++) {
        printf("%c", fileName[i]);
        if(i == 7) printf("%c", dir ? ' ' : '.');
        // not extension name for dir;
    }
}

int attr_check(char attr, int loc) {
    return attr & (1 << loc);
} 

void attr_set(char *attr, int loc) {
    *attr |= (1 << loc);
}

void display_entryInfo(struct entry *item) {
    if(attr_check(item->DIR_Attr, 1)) return ;
    // file not visible;
    
    display_fileName(item->DIR_Name, attr_check(item->DIR_Attr, 4));

    if(attr_check(item->DIR_Attr, 4)) {
        printf(" <DIR> "); // a sub-directory;
        printf("       ");
    } else {
        printf("       "); // a file;
        if(item->DIR_FileSize >= 1000) {
            printf("%3d,", item->DIR_FileSize / 1000);
        } else {
            printf("    ");
        }
        printf("%3d", item->DIR_FileSize % 1000);
    } // display its size (if it's a file);

    printf(" %4d ", item->DIR_FstClus);

    file_getDate(item->DIR_WrtDate); // display date of last change;
    file_getTime(item->DIR_WrtTime); // display time of last change;
} 

void display_dirInfo(char *sector, int cnt) {
    struct entry curEntry;
    for(int i = 0;i < cnt;i++) {
        memcpy(&curEntry, sector + i * sizeof(curEntry), sizeof(curEntry));
        if(!curEntry.DIR_Name[0])
            break; // root ends;
        if(curEntry.DIR_Name[0] == 0xE5) 
            continue; // deleted entry;
        display_entryInfo(&curEntry);
    }
} 

void display_fileContent(int startSec, int offset, int ifRoot) {
    system("cls");

    struct entry targetFile;
    memcpy(&targetFile, image + FAT12_get_secAdd(startSec, offset, ifRoot), entrySize);

    int nowSec, bytesLeft;
    char *fileSec = (char *)malloc(mbr.BPB_BytsPerSec);
    for(nowSec = targetFile.DIR_FstClus, bytesLeft = targetFile.DIR_FileSize;
        nowSec != 0xFFF;
        nowSec = FAT12_get_next(nowSec, 0)) {
        memcpy(fileSec, image + FAT12_get_secAdd(nowSec, 0, 0), mbr.BPB_BytsPerSec);

        for(int i = 0;i < Min(mbr.BPB_BytsPerSec, bytesLeft);i++)
            printf("%c", fileSec[i]);
        bytesLeft -= bytesLeft > mbr.BPB_BytsPerSec ? mbr.BPB_BytsPerSec : bytesLeft;
    }
    free(fileSec);
    printf("\n");
}

int  file_writeFile(int mode, struct entry *fileEntry, char *data, int dataSize) {
    // overwrite if mode == 1
    int nowSec, offset;
    if(mode) {
        // clear existing data;
        for(nowSec = fileEntry->DIR_FstClus;nowSec != 0xFFF;nowSec = FAT12_get_next(nowSec, 0)) 
            file_freeSec(nowSec);
        nowSec = fileEntry->DIR_FstClus, offset = 0;
    } else {
        // go to the first available location;
        for(nowSec = fileEntry->DIR_FstClus;
            FAT12_get_next(nowSec, 0) != 0xFFF; // stop before end of the file;
            nowSec = FAT12_get_next(nowSec, 0));
        // bytes in the last sector;
        offset = fileEntry->DIR_FileSize % mbr.BPB_BytsPerSec;
        // a full sector;
        if(fileEntry->DIR_FileSize && !offset) {
            FAT12_push_back_new_sector(nowSec);
            nowSec = FAT12_get_next(nowSec, 0);
        } 
    }
    
    // change file attributes;
    fileEntry->DIR_FileSize = mode ? dataSize : fileEntry->DIR_FileSize + dataSize;
    file_setDateAndTime(&fileEntry->DIR_WrtDate, &fileEntry->DIR_WrtTime);
    attr_set(&fileEntry->DIR_Attr, 5);
    
    // fill the following data;
    for(int i = 0;i < dataSize;i++, offset++) {
        if(offset == mbr.BPB_BytsPerSec) {
            // push back a new sector;
            if(FAT12_get_next(nowSec, 0) == 0xFFF)
                FAT12_push_back_new_sector(nowSec);
            nowSec = FAT12_get_next(nowSec, 0), offset = 0;
        }
        *(image + FAT12_get_secAdd(nowSec, 0, 0) + offset) = data[i];
    }
}

void file_loadFile(struct entry *file, char *data) {
    int nowSec, bytesLeft, i = 0;
    for(nowSec = file->DIR_FstClus, bytesLeft = file->DIR_FileSize;
        nowSec != 0xFFF;
        nowSec = FAT12_get_next(nowSec, 0), i++) {
        memcpy(data + i * mbr.BPB_BytsPerSec, image + FAT12_get_secAdd(nowSec, 0, 0), Min(bytesLeft, mbr.BPB_BytsPerSec));

        bytesLeft -= bytesLeft > mbr.BPB_BytsPerSec ? mbr.BPB_BytsPerSec : bytesLeft;
    }
}

void file_freeSec(int FAT12Index) {
    char *ptr = image + FAT12_get_secAdd(FAT12Index, 0, 0);
    memset(ptr, 0, mbr.BPB_BytsPerSec);
}

void file_loadEntry(struct entry *enp, int nowSec, int offset, int ifRoot) {
    enp->DIR_Name[0] = 0;
    if(ifRoot ? nowSec >= 33 : nowSec == 0xFFF)
        return ;
    // check if the location is valid;

    int nowAdd = FAT12_get_secAdd(nowSec, offset, ifRoot);
    memcpy(enp, image + nowAdd, sizeof(*enp));
    // load a file entry;
}

void file_moveEntry(int *sec, int *offset, int ifRoot) {
    if(*offset == mbr.BPB_BytsPerSec / entrySize - 1) {
        *sec = FAT12_get_next(*sec, ifRoot);
        *offset = 0;
    } else *offset = *offset + 1;
}

void file_setDateAndTime(unsigned short *DIR_WrtDate, unsigned short *DIR_WrtTime) {
    time_t rawTime; time(&rawTime);
    struct tm *timeInfo = localtime(&rawTime);

    *DIR_WrtDate = timeInfo->tm_mday + 
                  (timeInfo->tm_mon + 1 << 5) + 
                 ((timeInfo->tm_year - 80) << 9);
    *DIR_WrtTime = (timeInfo->tm_hour << 11) + 
                   (timeInfo->tm_min << 5);
}

void file_getDate(unsigned short DIR_WrtDate) {
    int year  = (DIR_WrtDate >> 9) + 1980, 
        month = ((DIR_WrtDate % (1 << 9)) >> 5), 
        date  = DIR_WrtDate % (1 << 5);
    printf(" %02u-%02u-%u  ", month, date, year);
}

void file_getTime(unsigned short DIR_WrtTime) {
    int hour = DIR_WrtTime >> 11,
        mins = (DIR_WrtTime % (1 << 11)) >> 5;
    printf("%02u:%02u%c\n", hour % 12, mins, hour > 12 ? 'p' : 'a');
} 

int file_checkName(char *fileName, int emptyAllowed) {
    int file = 0, l = strlen(fileName), i;
    if(l == 0) return emptyAllowed;
    if(l > 12) return 0;        // length restriction;
    if(fileName[0] == ' ') return 0; 
    // space not allowed in the first char;
    if(fileName[0] == '.') {
        if(l == 1 || fileName[1] == '.') return 1;
        else return 0;
    }

    for(i = 0;i < l;i++) {
        if(fileName[i] == '.') {
            if(!file) file = i;
            else return 0;           // only one dot is allowed;
        }
        if(fileName[i] < 0x20) return 0;
        for(int j = 0;j < fbListSize;j++) {
            if(fileName[i] == '.' && file) continue; // skip the dot for seperation;
            if(fileName[i] == forbiddedList[j]) 
                return 0;
        }
    }
    if(!file) {
        return l > 8 ? 0 : 2;
    } else {
        return (file > 8 || (l - file - 1) > 3) ? 0 : 1;
    }
}

void file_setName(char *nameSec, char *newName) {
    for(int i = 0;i < 11;i++) nameSec[i] = ' ';
    int l = strlen(newName), i, j;
    for(i = 0;i < l && newName[i] != '.';i++)
        nameSec[i] = newName[i];
    for(i++, j = 0;i < l;i++, j++)
        nameSec[8+j] = newName[i];
}

int file_strcmp(char *target, char *source, int dir) {
    int l = strlen(source), i, j;
    if(dir) { // if target is a dir, only match its prefix;
        for(i = 0;i < l;i++)
            if(target[i] != source[i])
                return 0;
        if(i < 8 && target[i] != ' ') return 0;
        // to ensure that two compared string hold the same length;
        return 1;
    } else { // if target is a file, match name and extension both;
        for(i = 0;i < l && source[i] != '.';i++) 
            if(target[i] != source[i])
                return 0;
        if(target[i] != ' ' || source[i] != '.') return 0;
        i++;
        for(j = 0;j < 3 && i+j < l;j++) 
            if(target[8+j] != source[i+j])
                return 0;
        if(i+j < l && target[8+j] != ' ') return 0;
        // same as above;
        return 2;
    }
}

void file_createFile(struct entry *curEntry, char *fileName, int dir, int FATp) {
    if(curEntry->DIR_Name[0] == 0xE5)  // a deleted file, free space first;
        FAT12_free_FAT_list(curEntry->DIR_FstClus);

    // allocate the first empty cluster for the new file;
    curEntry->DIR_FstClus = FAT12_firstAvailableFATp;
    FAT12_set_next(curEntry->DIR_FstClus, 0xFFF);
    file_freeSec(curEntry->DIR_FstClus);
    FAT12_get_next_available_FATp();

    file_setName(curEntry->DIR_Name, fileName); // set name;
    file_setDateAndTime(&curEntry->DIR_WrtDate, &curEntry->DIR_WrtTime); // set time;
    curEntry->DIR_FileSize = 0; // set file size;
    
    attr_set(&curEntry->DIR_Attr, 5); // Achieve Flag;
    if(dir) {
        attr_set(&curEntry->DIR_Attr, 4); // a directory;

        struct entry *linkEntry;
        linkEntry = (struct entry *)(image + FAT12_get_secAdd(curEntry->DIR_FstClus, 0, 0));
        memcpy(linkEntry, curEntry, entrySize);
        memset(linkEntry->DIR_Name, ' ', 11); linkEntry->DIR_Name[0] = '.';

        linkEntry = (struct entry *)(image + FAT12_get_secAdd(curEntry->DIR_FstClus, 1, 0));
        memcpy(linkEntry, curEntry, entrySize);
        memset(linkEntry->DIR_Name, ' ', 11); linkEntry->DIR_Name[0] = linkEntry->DIR_Name[1] = '.';
        linkEntry->DIR_FstClus = FATp;
    } // create the default "." and ".." files, only change their names and firstClus value;
}

struct dirNode dir_count_file(int FATp, int ifRoot) {
    struct dirNode dirRes;
    dirRes.fileCount = dirRes.memUsed = dirRes.totCount = 0;

    struct entry curEntry; curEntry.DIR_Name[0] = 0;
    int nowSec = FATp, offset = 0;
    while(file_loadEntry(&curEntry, nowSec, offset, ifRoot), curEntry.DIR_Name[0]) {
        if(curEntry.DIR_Name[0] != '.' && curEntry.DIR_Name[0] != 0xE5) {
            if(!attr_check(curEntry.DIR_Attr, 4))
                dirRes.fileCount += 1, dirRes.memUsed += curEntry.DIR_FileSize;
            dirRes.totCount++;
        }

        file_moveEntry(&nowSec, &offset, ifRoot);
    }
    return dirRes; 
}
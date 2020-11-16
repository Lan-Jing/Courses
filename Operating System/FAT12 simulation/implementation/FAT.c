#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "..\headers\fileSys.h"

// definition of two FAT sectors;
unsigned char FAT12_1[9 * 512];
unsigned char FAT12_2[9 * 512];

// definition of the first available FAT index;
unsigned short FAT12_firstAvailableFATp;

extern struct MBR mbr;

/*-----------------------------------------------------*/

void FAT12_recover(char *des, char *source) {
    memcpy(des, source, 9 * mbr.BPB_BytsPerSec);
} 

void FAT12_get_FAT(char *image, int readOrWrite) {
    if(readOrWrite) {
        memcpy(image + mbr.BPB_BytsPerSec, FAT12_1, 9 * mbr.BPB_BytsPerSec);
        memcpy(image + 10 * mbr.BPB_BytsPerSec, FAT12_2, 9 * mbr.BPB_BytsPerSec);
    } else {
        memcpy(FAT12_1, image + mbr.BPB_BytsPerSec, 9 * mbr.BPB_BytsPerSec);
        memcpy(FAT12_2, image + 10 * mbr.BPB_BytsPerSec, 9 * mbr.BPB_BytsPerSec);

        FAT12_firstAvailableFATp = 2;
        FAT12_get_next_available_FATp();
        // find the first available FAT sector;
    }
}

unsigned char *FAT12_get_validFAT() {
    unsigned char *FATp = FAT12_1;
    return FATp;
} 

int FAT12_get_next(int FAT12Index, int ifRoot) {
    if(ifRoot) return FAT12Index + 1; // root directory
    unsigned char *FATp = FAT12_get_validFAT();
    if(FATp == NULL) return -1;
    
    return FAT12Index % 2 ? (unsigned short)(FATp[3*FAT12Index/2] >> 4) + ((unsigned short)FATp[3*FAT12Index/2+1] << 4)
                          : (unsigned short)FATp[3*FAT12Index/2] + ((unsigned short)(FATp[3*FAT12Index/2+1] % (1 << 4)) << 8);
} 

void FAT12_set_next(int FAT12Index, int next) {
    if(FAT12Index % 2) {
        FAT12_1[3*FAT12Index/2+1] = FAT12_2[3*FAT12Index/2+1] = (next >> 4);
        FAT12_1[3*FAT12Index/2] &= 0b00001111, FAT12_2[3*FAT12Index/2] &= 0b00001111;
        FAT12_1[3*FAT12Index/2] = FAT12_2[3*FAT12Index/2] |= ((next % (1 << 4)) << 4);
    } else {
        FAT12_1[3*FAT12Index/2] = FAT12_2[3*FAT12Index/2] = next % (1 << 8);
        FAT12_1[3*FAT12Index/2+1] &= 0b11110000, FAT12_2[3*FAT12Index/2+1] &= 0b11110000;
        FAT12_1[3*FAT12Index/2+1] = FAT12_2[3*FAT12Index/2+1] |= (next >> 8);
    }
}

int FAT12_get_secAdd(int FAT12Index, int offset, int ifRoot) {
    return (ifRoot ? FAT12Index * mbr.BPB_BytsPerSec 
                   : (FAT12Index + 31) * mbr.BPB_BytsPerSec)
                   + entrySize * offset;
}

void FAT12_get_next_available_FATp() {
    if(FAT12_firstAvailableFATp == mbr.BPB_TotSec16 - 1) {
        printf("Running out of disk space!\nSaving file and exiting\n");
        image_close(imagePath);
        exit(1);
    }
    for(int index = FAT12_firstAvailableFATp + 1;index < mbr.BPB_TotSec16;index++) {
        if(!FAT12_get_next(index, 0)) {
            FAT12_firstAvailableFATp = index;
            return ;
        }
    }
}

int FAT12_push_back_new_sector(int FAT12Index) {
    FAT12_set_next(FAT12Index, FAT12_firstAvailableFATp);
    file_freeSec(FAT12_firstAvailableFATp);

    FAT12_set_next(FAT12_firstAvailableFATp, 0xFFF);
    FAT12_get_next_available_FATp();
}

void FAT12_free_FAT_list(int FAT12Index) {
    int now = FAT12Index, next;
    while(1) {
        next = FAT12_get_next(now, 0);
        FAT12_firstAvailableFATp = Min(FAT12_firstAvailableFATp, now);
        // maintain the first available FAT index for speedy access;
        if(now % 2) {
            FAT12_1[3*now/2] &= 0b00001111, FAT12_2[3*now/2] &= 0b00001111;
            FAT12_1[3*now/2+1] = FAT12_2[3*now/2+1] = 0;
        } else {
            FAT12_1[3*now/2] = FAT12_2[3*now/2] = 0;
            FAT12_1[3*now/2+1] &= 0b11110000, FAT12_2[3*now/2+1] &= 0b11110000;
        }
        file_freeSec(now);
        if(next == 0xFFF) break;
        now = next;
    }    
}
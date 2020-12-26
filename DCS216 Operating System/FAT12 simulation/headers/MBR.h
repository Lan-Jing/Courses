#ifndef MBR_H
#define MBR_H

#define db unsigned char 
#define dw unsigned short
#define dd unsigned int

#pragma pack(1)

struct MBR {
    db BS_jmpBoot[3]; // a jump instruction to MBR code;
    db BS_OEMName[8]; // Name of the manufacturer;
    dw BPB_BytsPerSec; // bytes per sector;
    db BPB_SecPerClus; // sector(s) per cluster;
    dw BPB_RsvdSecCnt; // sectors occupied by Boot record;
    db BPB_NumFATs; // number of FAT records;
    dw BPB_RootEntCnt; // Maximum number of root entries;
    dw BPB_TotSec16; // total sector;
    db BPB_Media; // Descirption char for media;
    dw BPB_FATSz16; // sector(s) pre FAT
    dw BPB_SecPerTrk; // sector(s) pre track;
    dw BPB_NumHeads; // number of mag-heads;
    dd BPB_HiddSec; // number of hidden sectors;
    dd BPB_TotSec32; // number of sectors;
    db BS_DrvNum; // drive index for interupt 13;
    db BS_Reserved1; // unused byte
    db BS_BootSig; // Boot signature;
    dd BS_VoID; // Volume index;
    db BS_VoILab[11]; // Volume Label;
    db BS_FileSysType[8]; // file system type;
    db MBR_BootCode[448]; // MBR code for booting the system;
    db MBR_endSig[2]; // magic numbers indicating a MBR record;
};

extern struct MBR mbr;

/*-----------------------------------------------*/

// load MBR into memory;
void MBR_get_MBR(char *image);

// validate key MBR information;
int MBR_check(struct MBR *mbr);

#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "..\headers\fileSys.h"

struct MBR mbr; // Only definition of the global MBR sector;

void MBR_get_MBR(char *image) {
    memcpy(&mbr, image, 512);
}

int MBR_check(struct MBR *mbr) {
    printf("Performing MBR validation:\n");
    
    if(strcmp(mbr->BS_FileSysType, "FAT12") || mbr->BPB_NumFATs != 2 || mbr->BPB_TotSec16 != 0xB40) {
        printf("Valid FAT12 system.\n");
    } else {
        printf("Not a valid FAT12 system.\n");
        return 0;
    }
    
    if(mbr->MBR_endSig[0] == 0x55 && mbr->MBR_endSig[1] == 0xAA) {
        printf("Valid MBR record.\n\n");
    } else {
        printf("Not a valid MBR record.\n");
        return 0;
    }

    char buffer[20];

    printf("Displaying key parameters:\n");
    memcpy(buffer, mbr->BS_VoILab, sizeof(mbr->BS_VoILab));
    buffer[sizeof(mbr->BS_VoILab)] = '\0';
    printf("Volume Label: %s\n", buffer);
    memcpy(buffer, mbr->BS_OEMName, sizeof(mbr->BS_OEMName));
    buffer[sizeof(mbr->BS_OEMName)] = '\0';
    printf("Manufacturer: %s\n", buffer);
    printf("Total Sectors: %d\n", (int)mbr->BPB_TotSec16);
    printf("Bytes per Sector: %d\n", (int)mbr->BPB_BytsPerSec);
    printf("Sector(s) per Cluster: %d\n", (int)mbr->BPB_SecPerClus);
    printf("number of FATs: %d\n\n", (int)mbr->BPB_NumFATs);

    return 1;
}
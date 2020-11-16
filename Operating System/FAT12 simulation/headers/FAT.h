#ifndef FAT_H
#define FAT_H

// global declaration;
extern unsigned char FAT12_1[9 * 512];
extern unsigned char FAT12_2[9 * 512];

// pointed to the first availdble FAT sector;
extern unsigned short FAT12_firstAvailableFATp;

/*-----------------------------------------------*/

// load FAT into memory;
void FAT12_get_FAT(char *image, int readOrWrite);

// get a valid FAT, simplified.
unsigned char *FAT12_get_validFAT();

// recover from another;
void FAT12_recover(char *des, char *source);

// get FAT content in the given location;
int  FAT12_get_next(int FAT12Index, int ifRoot);

// set FAT content in the given loation;
void  FAT12_set_next(int FAT12Index, int next);

// get image address of the given FAT index;
int  FAT12_get_secAdd(int FAT12Index, int offset, int ifRoot);

// return the next availalbe FAT sector;
void FAT12_get_next_available_FATp();

// link a new FAT sector to the back of the list and return its index;
int FAT12_push_back_new_sector(int FAT12Index);

// free FAT entries following the given FAT sector;
void FAT12_free_FAT_list(int FAT12Index);

#endif
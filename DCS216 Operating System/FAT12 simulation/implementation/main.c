#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "..\headers\fileSys.h"

#pragma pack(1)

int main (int argv, char *argc[]) {
    imagePath = argc[1];

    if(image_open(imagePath) == -1) {
        return 0;
    } // open the image file;
    
    help(); // output simple help information;

    char *commandLine = (char *)malloc(MaxCommandSize * sizeof(char));
    while(1) {
        printf("\\%s> ", curPath);
        fgets(commandLine, MaxCommandSize - 1, stdin);
        commandLine[strlen(commandLine)-1] = '\0'; // replace the enter;
        if(!strcmp(commandLine, "exit")) 
            break;
        cmd(commandLine);
    }
    free(commandLine);

    image_close(imagePath);
    return 0;
}
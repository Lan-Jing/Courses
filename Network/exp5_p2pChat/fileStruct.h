#pragma once
#pragma pack(1) // no alignment

#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>

#define packSize (1 << 10)
#define nameSize 50

struct fileStruct {
	char type; // 0 file/ 1 text
	long long size;
	char fileName[50];
	char data[packSize - (1 + 8 + 50)];
};

struct textStruct {
	char type;
	long long size;
	char data[packSize - (1 + 8)];
};

int get_fileSize(FILE *fp);

// e:\\filename -> fileName
void remove_path(char *fileName);

// fileName.ext -> fileName,newPart.ext
void strcat_noext(char *fileName, char *newPart);

int connectSock(const char *hostName, const char *port);
void sendBuffer(int *sock);
void cmd(SOCKET sock, char *command);
void sendFile(SOCKET sock, char *fileName);
void sendText(SOCKET sock, char *text);
void recvData(SOCKET *sock);
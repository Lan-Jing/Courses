#include <iostream>
using namespace std;
#include "fileStruct.h"
#include <WS2tcpip.h>
#include <WSPiApi.h>

extern int terminateFlag;
extern char tarFilePath[nameSize];

int connectSock(const char *hostName, const char *port) {
	// set hint to the socket type;
	struct addrinfo hints, *head;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	printf("Connecting to %s, port %s\n", hostName, port);

	// resolves hostname and port number then returns a linked list for possible connection;
	int getInfoRes = getaddrinfo(hostName, port, &hints, &head);
	if(getInfoRes) {
		printf("Failed to resolve host information %d\n", getInfoRes);
		return -1;
	}

	// try to find a socket capable to be connected;
	int sock = 0;
	struct addrinfo *addr;
	for(addr = head; addr != NULL; addr = addr->ai_next) {
		sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
		// runtime error;
		if(sock == SOCKET_ERROR) break;
		// a successful connection;
		if(connect(sock, addr->ai_addr, addr->ai_addrlen) == 0) break;

		// failed to connect;
		closesocket(sock);
		sock = -1;
	}

	freeaddrinfo(head);
	if(sock == -1) {
		printf("Failed to connect to the server, error: %d\n", WSAGetLastError());
	} else {
		struct sockaddr_in *sockaddr_ipv4 = (struct sockaddr_in *)addr->ai_addr;
		printf("Connection to ip:%s established\n", inet_ntoa(sockaddr_ipv4->sin_addr));
	}
	return sock;
}

void cmd(SOCKET sock, char *command) {
	char option[100], para[100];
	memset(option, 0, sizeof(option));
	memset(para, 0, sizeof(para));

	int flag = 0;
	// find the space between two words;
	for(int i = 0; i < strlen(command); i++)
		if(!flag && command[i] == ' ') flag = i;
	// only one word, treated as text;
	if(!flag) {
		// exiting the program;
		if(!strcmp(command, "quit")) {
			terminateFlag = 1;
			return;
		} else {
			memcpy(para, option, strlen(option));
			memset(option, 0, sizeof(option));
			memcpy(option, "chat", 5);
		}
	}
	memcpy(option, command, flag);
	memcpy(para, command + flag + 1, strlen(command) - flag - 1);
	// execute functions;
	if(!strcmp(option, "rdir")) {
		memset(tarFilePath, 0, sizeof(tarFilePath));
		memcpy(tarFilePath, para, strlen(para));
		if(tarFilePath[strlen(para) - 1] != '\\')
			strcat_s(tarFilePath, "\\");
		printf("Target file path set as: %s\n", tarFilePath);
	} else if(!strcmp(option, "chat")) {
		sendText(sock, para);
	} else if(!strcmp(option, "send")) {
		sendFile(sock, para);
	} else
		printf("Cannot resolve command\n");
}

void sendBuffer(int *sock) {
	while(1) {
		if(terminateFlag) return;

		char text[2 * nameSize];
		gets_s(text, 2 * nameSize);
		cmd(*sock, text);
	}
}

void sendFile(SOCKET sock, char *fileName) {
	FILE *srcfp = NULL;
	fopen_s(&srcfp, fileName, "rb");
	if(srcfp == NULL) {
		printf("Failed to open srouce file\n");
		return;
	}

	int l = get_fileSize(srcfp), nowp = 0;
	int packCount = (l + 59 - 1) / packSize + 1;
	char pack[packSize];
	for(int i = 0; i < packCount; i++) {
		int freeSpace = i ? min(l, packSize) : min(l, packSize - 59);
		if(!i) {
			((struct fileStruct *)pack)->type = 0, ((struct fileStruct *)pack)->size = l;
			remove_path(fileName);
			memcpy(((struct fileStruct *)pack)->fileName, fileName, strlen(fileName)+1);
			fread(pack + 59, 1, freeSpace, srcfp);
		} else {
			fread(pack, 1, freeSpace, srcfp);
		}
		l -= freeSpace;

		int sendRes = send(sock, pack, i ? freeSpace : freeSpace + 59, 0);
		if(sendRes <= 0) {
			if(!sendRes) {
				printf("Failed to send file package, error:%d\n", WSAGetLastError());
			}
			return;
		}
	}
	fclose(srcfp);
}

void sendText(SOCKET sock, char *text) {
	int l = strlen(text), nowp = 0;
	int packCount = (l + 9 - 1) / packSize + 1;
	char pack[packSize];
	for(int i = 0; i < packCount; i++) {
		int freeSpace = i ? min(l, packSize) : min(l, packSize - 9);
		if(!i) {
			((struct textStruct *)pack)->type = 1, ((struct textStruct *)pack)->size = l;
			memcpy(pack + 9, text + nowp, freeSpace);
		} else {
			memcpy(pack, text + nowp, freeSpace);
		}
		nowp += freeSpace, l -= freeSpace;

		int sendRes = send(sock, pack, i ? freeSpace : freeSpace + 9, 0);
		if(sendRes <= 0) {
			if(!sendRes) {
				printf("Failed to send text package, error:%d\n", WSAGetLastError());
			}
			return;
		}
	}
}

// record duplicated file names;
map<string, int> Map;
void recvData(SOCKET *sock) {
	char pack[packSize];
	int packCount = 0, type, size;

	while(1) {
		if(terminateFlag) return;
		// receive the first package;
		memset(pack, 0, sizeof(pack));
		int recvRes = recv(*sock, pack, packSize, 0);
		if(recvRes <= 0) {
			if(recvRes == SOCKET_ERROR) {
				printf("Failed to receive data, error:%d\n", WSAGetLastError());
			}
			return;
		}
		// get data type and size;
		type = pack[0];
		size = ((struct textStruct *)pack)->size;
		if(type) {
			// text
			packCount = (size + 9 - 1) / packSize + 1;
			for(int i = 9; i < min(size + 9, packSize); i++)
				putchar(pack[i]);
			size -= min(packSize - 9, size);
			for(int i = 0; i < packCount - 1; i++) {
				memset(pack, 0, sizeof(pack));
				int recvRes = recv(*sock, pack, packSize, 0);
				if(recvRes <= 0) {
					if(recvRes == SOCKET_ERROR) 
						printf("Failed to receive data, error:%d\n", WSAGetLastError());
					return;
				} else {
					for(int i = 0; i < min(size, packSize); i++)
						putchar(pack[i]);
					size -= min(packSize, size);
				}
			}
			printf("\n");
		} else {
			// file
			packCount = (size + 59 - 1) / packSize + 1;
			char fileName[nameSize], filePath[nameSize];
			memset(fileName, 0, sizeof(fileName)), memset(filePath, 0, sizeof(filePath));
			memcpy(fileName, ((struct fileStruct *)pack)->fileName, strlen(((struct fileStruct *)pack)->fileName)+1);
			// deal with duplicated files;
			if(Map.count(fileName)) {
				char index[10];
				sprintf_s(index, "(%d)", Map[fileName]++);
				strcat_noext(fileName, index);
			} else 
				Map[fileName]++;
			// get proper file path;
			memcpy(filePath, tarFilePath, sizeof(tarFilePath));
			strcat_s(filePath, fileName);
			printf("Receiving file:%s, size:%d to %s\n", fileName, size, filePath);
			// open file;
			FILE *tarfp = NULL;
			fopen_s(&tarfp, filePath, "wb");
			if(tarfp == NULL) {
				printf("Failed to open target file\n");
				return;
			}
			// receive the whole file
			for(int i = 59; i < min(size + 59, packSize); i++)
				fwrite(pack + i, 1, 1, tarfp);
			size -= min(packSize - 59, size);
			for(int i = 0; i < packCount - 1; i++) {
				memset(pack, 0, sizeof(pack));
				int recvRes = recv(*sock, pack, packSize, 0);
				if(recvRes <= 0) {
					if(recvRes == SOCKET_ERROR) 
						printf("Failed to receive file, error:%d\n", WSAGetLastError());
					return;
				} else {
					for(int i = 0; i < min(size, packSize); i++)
						fwrite(pack + i, 1, 1, tarfp);
					size -= min(packSize, size);
				}
			}
			fclose(tarfp);
		}
	}
}
#include <winsock2.h>
#include <WS2tcpip.h>
#include <WSPiApi.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <Helper.h>

// char buffer used by send/recv processes;
#define bufSize 1000
char inBuffer[bufSize+1], outBuffer[bufSize+1], fileBuffer[bufSize+1], fileName[FILE_NAME_LEN], tmpBuffer[bufSize+1];
// target information
char* hostName, * port;
// handle for file returned;
FILE* fp;

// signal for terminating the programm;
int terminateFlag, fileFlag = 1, sendFlag;

// socket handle of the current connection;
int controlSock, dataSock;

int connectSock(const char* hostName, const char* port) {
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
    struct addrinfo* addr;
    for(addr = head;addr != NULL;addr = addr->ai_next) {
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
    }
    else {
        struct sockaddr_in *sockaddr_ipv4 = (struct sockaddr_in *)addr->ai_addr;
        printf("Connection to ip:%s established\n", inet_ntoa(sockaddr_ipv4->sin_addr));
    } 
    return sock;
}

void sendBuffer(int *sock, char *buffer) {
    // send the message;
    int sendRes = send(*sock, buffer, strlen(buffer), 0);
    if(sendRes <= 0) {
        if(sendRes == SOCKET_ERROR) {
            printf("\nFailed to send message to server, error: %d\n", WSAGetLastError());
            terminateFlag = 1;
            return;
        }
    }
    printf("%s", buffer);
}

// only print message from control link
void recvBuffer(int *sock) {
    while(1) {
        if(terminateFlag) return ;

        memset(tmpBuffer, 0, sizeof(tmpBuffer));
        int recvRes = recv(*sock, tmpBuffer, bufSize, 0);
        memset(inBuffer, 0, sizeof(inBuffer));
        memcpy(inBuffer, tmpBuffer, strlen(tmpBuffer));
        if(recvRes <= 0) {
            // runtime error;
            if(recvRes == SOCKET_ERROR) {
                printf("\nFailed to receive message from server, error: %d\n", WSAGetLastError());
            }

            // connection closed;
            terminateFlag = 1;
            return ;
        } else if(recvRes) {
            // output the message;
            for(int i = 0; i < strlen(inBuffer); i++)
                putchar(inBuffer[i]);
            // transfer completed;
            if(!strncmp(inBuffer, "226", 3))
                fileFlag = 1;
        }
    }
}

void recvFile(int *sock) {
    // set proper file path;
    char filePath[FILE_NAME_LEN];
    memcpy(filePath, "E:\\", 4);
    strcat_s(filePath, fileName);
    // try to open file;
    fp = NULL;
    fopen_s(&fp, filePath, "wb");
    if(fp == NULL) {
        printf("Failed to open file\n");
        terminateFlag = 1;
        return ;
    }
    // receive data;
    while(1) {
        if(terminateFlag || fileFlag) break;

        memset(fileBuffer, 0, sizeof(fileBuffer));
        int recvRes = recv(*sock, fileBuffer, bufSize, 0);
        if(recvRes <= 0) {
            if(recvRes == SOCKET_ERROR) {
                printf("\nFailed to receive data from server, error:%d\n", WSAGetLastError());
            }
            terminateFlag = 1;
            break;
        } else if(recvRes) {
            if(fwrite(fileBuffer, sizeof(char), strlen(fileBuffer), fp) < strlen(fileBuffer)) {
                printf("Failed to write in file\n");
                terminateFlag = 1;
                break;
            }
        }
    }
    fclose(fp);
}

int main(int argc, char *argv[]) {
    WORD WSVersionRequest = MAKEWORD(2, 2);
    WSADATA WSAData;
    
    // set up socket environment
    int startRes = WSAStartup(WSVersionRequest, &WSAData);
    if(startRes) {
        printf("Winsock startup failed with error: %d\n", startRes);
        return 1;
    } else {
        printf("Winsock %d.%d startup completed\n", HIBYTE(WSAData.wVersion), LOBYTE(WSAData.wVersion));
    }

    // only connect to ftp port;
    hostName = argv[1], port = (char *)"21";
    controlSock = connectSock(hostName, port);
    
    // create sub-threads;
    HANDLE recvThread = 0, dataThread = 0;
    // open channel for control message;
    recvThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)recvBuffer, &controlSock, 0, NULL);

    // establish control link with the server;
    sendBuffer(&controlSock, (char *)"user net\r\n");
    sendBuffer(&controlSock, (char *)"pass 123456\r\n");
    sendBuffer(&controlSock, (char *)"pasv\r\n");

    while(strncmp(inBuffer, "227", 3));

    // now the information of data link stays in inBuffer;
    int ip1, ip2, ip3, ip4, port1, port2;
    char *pos = strchr(inBuffer, '(');
    port = (char *)malloc(10);

    sscanf_s(pos + 1, "%d,%d,%d,%d,%d,%d", &ip1, &ip2, &ip3, &ip4, &port1, &port2);
    port1 = (port1 << 8) + port2;
    sprintf(port, "%d", port1);
    // establish data link
    dataSock = connectSock(hostName, port);
    free(port);

    // set proper file name;
    memset(fileName, 0, sizeof(fileName));
    memcpy(fileName, argv[2], strlen(argv[2]));
    get_fileName(fileName);

    // open channel for data message;
    fileFlag = 0;
    dataThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)recvFile, &dataSock, 0, NULL);

    memset(outBuffer, 0, sizeof(outBuffer));
    sprintf_s(outBuffer, "retr %s\r\n", argv[2]);
    sendBuffer(&controlSock, outBuffer);

    // wait until the flag is set;
    while(!fileFlag);

    // transfer completed;
    sendBuffer(&controlSock, (char *)"quit\r\n");

    terminateFlag = 1;
    WaitForSingleObject(recvThread, INFINITE);
    CloseHandle(dataThread);
    CloseHandle(recvThread);

    closesocket(dataSock);
    closesocket(controlSock);
    WSACleanup();
    return 0;
}
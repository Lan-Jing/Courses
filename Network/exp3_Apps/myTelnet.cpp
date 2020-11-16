#include <winsock2.h>
#include <WS2tcpip.h>
#include <WSPiApi.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#include "ansi_escapes.h"

// char buffer used by send/recv processes;
#define bufSize 10000
char inBuffer[bufSize+1], outBuffer[bufSize+1], tmpBuffer[bufSize+1];
// target information
char* hostName, * port;

// mutex protecting the buffer; 
HANDLE bufferMutex ,
// handle pointed to the console;
       consoleHandle;

// signal for terminating the programm;
int terminateFlag;

// socket handle of the current connection;
int sock;

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
        setTextColor(RED_TXT);
        printf("Failed to resolve host information %d\n", getInfoRes);
        resetColor();
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
        setTextColor(RED_TXT);
        printf("Failed to connect to the server, error: %d\n", WSAGetLastError());
        resetColor();
    }
    else {
        struct sockaddr_in *sockaddr_ipv4 = (struct sockaddr_in *)addr->ai_addr;
        setTextColor(GREEN_TXT);
        printf("Connection to ip:%s established\n", inet_ntoa(sockaddr_ipv4->sin_addr));
        resetColor();
    } 
    return sock;
}

void sendBuffer(int *sock) {
    while(1) {
        if(terminateFlag) return ;
        memset(outBuffer, 0, sizeof(outBuffer));
        memset(tmpBuffer, 0, sizeof(tmpBuffer));

        int flag = 0;
        while (1) {
            gets_s(tmpBuffer, bufSize);
            // exit the program;
            if (!strcmp(tmpBuffer, "exit")) {
                shutdown(*sock, SD_SEND);
                closesocket(*sock);
                WSACleanup();
                exit(0);
            }
            strcat_s(outBuffer, tmpBuffer);
            if (!strlen(tmpBuffer)) {
                if (flag) {
                    flag = 0;
                    break;
                } else {
                    flag++;
                }
            } else 
                strcat_s(outBuffer, "\r\n");
        }
        // send the message;
        int sendRes = send(*sock, outBuffer, strlen(outBuffer), 0);
        if(sendRes <= 0) {
            if(sendRes == SOCKET_ERROR) {
                setTextColor(RED_TXT);
                printf("\nFailed to send message to server, error: %d\n", WSAGetLastError());
                resetColor();
            }
            terminateFlag = 1;
            return ;
        }
    }
}

void recvBuffer(int *sock) {
    while(1) {
        if(terminateFlag) return ;

        memset(inBuffer, 0, sizeof(inBuffer));
        int recvRes = recv(*sock, inBuffer, bufSize, 0);

        if(recvRes <= 0) {
            // runtime error;
            if(recvRes == SOCKET_ERROR) {
                setTextColor(RED_TXT);
                printf("\nFailed to receive message from server, error: %d\n", WSAGetLastError());
                resetColor();
            }

            // connection closed;
            terminateFlag = 1;
            return ;
        } else if(recvRes) {
            // output the message;
            for (int i = 0; i < strlen(inBuffer); i++)
                putchar(inBuffer[i]);
        }
    }
}

int main(int argc, char *argv[]) {
    consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    setupConsole();

    WORD WSVersionRequest = MAKEWORD(2, 2);
    WSADATA WSAData;
    
    // set up socket environment
    int startRes = WSAStartup(WSVersionRequest, &WSAData);
    if(startRes) {
        setTextColor(RED_TXT);
        printf("Winsock startup failed with error: %d\n", startRes);
        resetColor();
        return 1;
    } else {
        setTextColor(GREEN_TXT);
        printf("Winsock %d.%d startup completed\n", HIBYTE(WSAData.wVersion), LOBYTE(WSAData.wVersion));
        resetColor();
    }

    hostName = argv[1], port = argv[2];
    sock = connectSock(hostName, port);
    
    // create sub-threads ;
    HANDLE recvThread, sendThread;
    // create the mutex protecting the output buffer;
    bufferMutex = CreateMutex(NULL, FALSE, NULL);

    // use two seperate thread for send/recv;
    sendThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)sendBuffer, &sock, 0, NULL);
    recvThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)recvBuffer, &sock, 0, NULL);

    WaitForSingleObject(sendThread, INFINITE);
    WaitForSingleObject(recvThread, INFINITE);
    CloseHandle(recvThread);
    CloseHandle(sendThread);

    shutdown(sock, SD_SEND);
    closesocket(sock);
    WSACleanup();
    return 0;
}
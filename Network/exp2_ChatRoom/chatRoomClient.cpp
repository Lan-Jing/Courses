#include <Windows.h>
#include <winsock2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#include "ansi_escapes.h"

#pragma comment(lib, "ws2_32.lib");

typedef int __stdcall (*WSAStartup_p) (WORD, LPWSADATA);
typedef SOCKET __stdcall (*socket_p) (int, int, int);
typedef int __stdcall (*closesocket_p) (SOCKET);
typedef int __stdcall (*WSAGetLastError_p) ();
typedef int __stdcall (*WSACleanup_p) ();
typedef u_short __stdcall (*htons_p) (u_short);
typedef unsigned long __stdcall (*inet_addr_p) (const char *);
typedef int __stdcall (*connect_p) (SOCKET, const sockaddr *, int);
typedef int __stdcall (*send_p) (SOCKET, const char *, int, int);
typedef int __stdcall (*recv_p) (SOCKET, char *, int, int);

WSAStartup_p WSAStartupp;
socket_p socketp;
closesocket_p closesocketp;
WSAGetLastError_p WSAGetLastErrorp;
WSACleanup_p WSACleanupp;
htons_p htonsp;
inet_addr_p inet_addrp;
connect_p connectp;
send_p sendp;
recv_p recvp;

// char buffer used by send/recv processes;
#define bufSize 100
char inBuffer[bufSize+1], outBuffer[bufSize+1];

// mutex protecting the buffer; 
HANDLE bufferMutex ,
// handle pointed to the console;
       consoleHandle;

// signal for terminating the programm;
int terminateFlag;

struct connectNode {
    int TCPClient;
    struct sockaddr_in targetAddr;
};

int connectSock(struct connectNode *con) {
    int connectRes = connectp(con->TCPClient, (struct sockaddr*)(&con->targetAddr), sizeof(con->targetAddr));
    
    WaitForSingleObject(bufferMutex, INFINITE);
    
    if(connectRes == SOCKET_ERROR) {
        setTextColor(RED_TXT);
        printf("Failed to connect to the TCP server, error: %d\n", WSAGetLastErrorp());
        resetColor();
    } else {
        setTextColor(GREEN_TXT);
        printf("Connection completed\n");
        resetColor();
    }
    
    ReleaseMutex(bufferMutex);
    return connectRes;
}

void sendBuffer(struct connectNode *con) {
    while(1) {
        if(terminateFlag) return ;
        memset(outBuffer, 0, sizeof(outBuffer));

        WaitForSingleObject(bufferMutex, INFINITE);
        setTextColor(GREEN_TXT);
        printf("\n\n<< ");
        resetColor();
        ReleaseMutex(bufferMutex);

        char tmpCh[2]; tmpCh[1] = '\0';
        while(1) {
            tmpCh[0] = getch();
            if(tmpCh[0] == 13) break;
            else {
                if(tmpCh[0] == 8 && strlen(outBuffer)) {
                    moveLeft(1);
                    clearLineToRight();
                    outBuffer[strlen(outBuffer)-1] = '\0';
                    continue; 
                } else if(32 <= tmpCh[0] && tmpCh[0] <= 126){
                    putchar(tmpCh[0]);
                    strcat(outBuffer, tmpCh);
                }
            }
        }
        if(!strcmp(outBuffer, "exit")) {
            terminateFlag = 1;
            return ;
        }
        int sendRes = sendp(con->TCPClient, outBuffer, strlen(outBuffer)+1, 0);
        if(sendRes <= 0) {
            WaitForSingleObject(bufferMutex, INFINITE);
            if(sendRes == SOCKET_ERROR) {
                setTextColor(RED_TXT);
                printf("\nFailed to send message to server, error: %d\n", WSAGetLastErrorp());
                resetColor();
            }
            ReleaseMutex(bufferMutex);
            terminateFlag = 1;
            return ;
        }
    }
}

void recvBuffer(struct connectNode *con) {
    while(1) {
        if(terminateFlag) return ;
        memset(inBuffer, 0, sizeof(inBuffer));

        int recvRes = recvp(con->TCPClient, inBuffer, bufSize, 0);

        WaitForSingleObject(bufferMutex, INFINITE);
        if(recvRes <= 0) {
            if(recvRes == SOCKET_ERROR) {
                setTextColor(RED_TXT);
                printf("\nFailed to receive message from server, error: %d\n", WSAGetLastErrorp());
                resetColor();
            }
            ReleaseMutex(bufferMutex);

            terminateFlag = 1;
            return ;
        } else if(recvRes) {
            if(strlen(outBuffer)) {
                setTextColor(RED_TXT);
                printf("\nBuffer above remains, continue in the next input zone");
                resetColor();
            }

            setTextColor(GREEN_TXT);
            printf("\n>> ");
            resetColor();
            printf("%s", inBuffer);

            setTextColor(GREEN_TXT);
            printf("\n<< ");
            resetColor();
            printf("%s", outBuffer);
        }

        ReleaseMutex(bufferMutex);
    }
}

int main() {
    consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    setupConsole();

    // load dynamic library;
    HMODULE hDll = LoadLibrary("ws2_32.dll");
    if(hDll == NULL) {
        printf("fail to load Winsock Dll\n");
        return 1;
    }
    WSAStartupp = (WSAStartup_p)GetProcAddress(hDll,"WSAStartup");
    socketp = (socket_p)GetProcAddress(hDll,"socket");
    closesocketp = (closesocket_p)GetProcAddress(hDll,"closesocket");
    WSAGetLastErrorp = (WSAGetLastError_p)GetProcAddress(hDll,"WSAGetLastError");
    WSACleanupp = (WSACleanup_p)GetProcAddress(hDll,"WSACleanup");
    htonsp = (htons_p)GetProcAddress(hDll,"htons");
    inet_addrp = (inet_addr_p)GetProcAddress(hDll,"inet_addr");
    connectp = (connect_p)GetProcAddress(hDll,"connect");
    sendp = (send_p)GetProcAddress(hDll,"send");
    recvp = (recv_p)GetProcAddress(hDll,"recv");

    WORD WSVersionRequest = MAKEWORD(2, 2);
    WSADATA WSAData;
    
    // set up socket environment
    int startRes = WSAStartupp(WSVersionRequest, &WSAData);
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

    // create a client socket;
    int TCPClient = socketp(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(TCPClient == INVALID_SOCKET) {
        setTextColor(RED_TXT);
        printf("Failed to initialize TCP client socket, error: %d\n", WSAGetLastErrorp());
        resetColor();
        WSACleanupp();
        return 1;
    } else {
        setTextColor(GREEN_TXT);
        printf("TCP client socket successfully initialized\n\n");
        resetColor();
    }

    // create sub-threads ;
    HANDLE recvThread, sendThread;
    // create the mutex protecting the output buffer;
    bufferMutex = CreateMutex(NULL, FALSE, NULL);

    // set connection information;
    struct sockaddr_in targetAddr;
    memset(&targetAddr, 0, sizeof(targetAddr));
    targetAddr.sin_family = AF_INET;
    // targetAddr.sin_addr.s_addr = inet_addrp("127.0.0.1");
    // targetAddr.sin_port = htonsp(55555);
    targetAddr.sin_addr.s_addr = inet_addrp("106.15.75.209");
    targetAddr.sin_port = htonsp(50500);

    // try to connect to the target;
    struct connectNode target;
    target.targetAddr = targetAddr, target.TCPClient = TCPClient;
    if(connectSock(&target) < 0) {
        closesocketp(TCPClient);
        WSACleanupp();
        return 1;
    }

    // use two seperate thread for send/recv;
    sendThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)sendBuffer, &target, 0, NULL);
    recvThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)recvBuffer, &target, 0, NULL);

    WaitForSingleObject(sendThread, INFINITE);
    WaitForSingleObject(recvThread, INFINITE);

    CloseHandle(recvThread);
    CloseHandle(sendThread);

    closesocketp(TCPClient);
    WSACleanupp();

    return 0;
}
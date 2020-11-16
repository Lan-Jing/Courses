#include <Windows.h>
#include <winsock2.h>
#include <conio.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "ansi_escapes.h"

#pragma comment(lib, "ws2_32.lib");

typedef int __stdcall (*WSAStartup_p) (WORD, LPWSADATA);
typedef SOCKET __stdcall (*socket_p) (int, int, int);
typedef int __stdcall (*closesocket_p) (SOCKET);
typedef int __stdcall (*WSAGetLastError_p) ();
typedef int __stdcall (*WSACleanup_p) ();
typedef int __stdcall (*bind_p) (SOCKET, const struct sockaddr *, int);
typedef u_short __stdcall (*htons_p) (u_short);
typedef u_short __stdcall (*ntohs_p) (u_short);
typedef char * __stdcall (*inet_ntoa_p) (in_addr in);
typedef int __stdcall (*connect_p) (SOCKET, const struct sockaddr *, int);
typedef int __stdcall (*send_p) (SOCKET, const char *, int, int);
typedef int __stdcall (*recv_p) (SOCKET, char *, int, int);
typedef int __stdcall (*listen_p) (SOCKET, int);
typedef SOCKET __stdcall (*accept_p) (SOCKET, struct sockaddr *, int *);

WSAStartup_p WSAStartupp; 
socket_p socketp; 
closesocket_p closesocketp;
WSAGetLastError_p WSAGetLastErrorp;
WSACleanup_p WSACleanupp;
bind_p bindp;
htons_p htonsp;
ntohs_p ntohsp;
inet_ntoa_p inet_ntoap;
connect_p connectp; 
send_p sendp; 
recv_p recvp; 
listen_p listenp; 
accept_p acceptp;

#define bufSize 100
#define queSize 100

struct connectNode {
    int TCPClient, index;
    struct sockaddr_in sourceAddr;
    HANDLE recvThread;
}connVec[queSize];
int head, tail;

// mutex to protect pointers head/tail;
HANDLE ptrMutex;

// flag indicating that server closes;
int terminateFlag;

int Max(int a, int b) { return a < b ? b : a; }
int Min(int a, int b) { return a < b ? a : b; }

// get the first empty node in the list;
void getHead(int *index) {
    *index = *index + 1;
    for(int i = *index;i < queSize;i++) {
        *index = i;
        if(connVec[i].TCPClient == 0) return ;
    }
}

// get the last non-empty node in the list;
void getTail(int *index) {
    *index = *index - 1;
    for(int i = *index;i >= 0;i--) {
        if(connVec[i].TCPClient == 0) continue;
        *index = i;
        return ;
    }
}

void sendAll(struct connectNode *conn, char *outBuffer) {
    for(int i = 0;i <= tail;i++) {
        if(i == conn->index) continue;
        sendp(connVec[i].TCPClient, outBuffer, strlen(outBuffer)+1, 0);
    }
}

void WINAPI recvAndSend(struct connectNode *conn) {
    char buffer[bufSize+1], outBuffer[bufSize+1];
    
    while(1) {
        if(terminateFlag) {
            CloseHandle(conn->recvThread);
            return ;
        }
        int recvRes = recvp(conn->TCPClient, buffer, bufSize, 0);
        
        // protect global data from data race;
        WaitForSingleObject(ptrMutex, INFINITE);
        if(recvRes <= 0) {
            // error running recv();
            if(recvRes < 0) {
                setTextColor(RED_TXT);
                int errorCode = WSAGetLastErrorp();
                printf("Failed to receive message, error: %d\n", errorCode);
                if(errorCode == 10054)
                    printf("Client with:\nIP %s\nport %d\nhas left the chat room\n\n",
                           inet_ntoap(conn->sourceAddr.sin_addr),
                           ntohsp(conn->sourceAddr.sin_port));
                resetColor();
            }

            // the client quits, node to be freed;
            closesocketp(conn->TCPClient);
            conn->TCPClient = 0;
            // reset the head/tail pointer;
            head = Min(head, conn->index);
            if(conn->index == tail)
                getTail(&tail);

            ReleaseMutex(ptrMutex);
            
            // exit the thread;
            CloseHandle(conn->recvThread);
            return ;
        } else {
            // output the received text;
            setTextColor(GREEN_TXT);
            printf("\n>>");
            resetColor();

            // then send to all other clients;
            time_t recvTime = time(NULL);

            outBuffer[0] = '\0';
            char *tmp = (char *)malloc(20 * sizeof(char));

            strcat(outBuffer, "\nClient IP: ");
            strcat(outBuffer, (const char*)inet_ntoap(conn->sourceAddr.sin_addr));
            strcat(outBuffer, "\nClient port: ");
            itoa(ntohsp(conn->sourceAddr.sin_port), tmp, 10);
            strcat(outBuffer, tmp);
            strcat(outBuffer, "\nServer time: ");
            tmp = ctime((const time_t*)&recvTime);
            strcat(outBuffer, tmp);
            strcat(outBuffer, "\n");
            strcat(outBuffer, buffer);
            printf("%s\n", outBuffer);
            free(tmp);

            sendAll(conn, outBuffer);

            ReleaseMutex(ptrMutex);
        }
    }
}

int main() {    
    setupConsole();

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
    bindp = (bind_p)GetProcAddress(hDll,"bind");
    htonsp = (htons_p)GetProcAddress(hDll,"htons");
    ntohsp = (ntohs_p)GetProcAddress(hDll,"ntohs");
    inet_ntoap = (inet_ntoa_p)GetProcAddress(hDll,"inet_ntoa");
    connectp = (connect_p)GetProcAddress(hDll,"connect");
    sendp = (send_p)GetProcAddress(hDll,"send");
    recvp = (recv_p)GetProcAddress(hDll,"recv");
    listenp = (listen_p)GetProcAddress(hDll,"listen");
    acceptp = (accept_p)GetProcAddress(hDll,"accept");
    
    // start up the socket environment;
    WORD WSVersionRequest = MAKEWORD(2,2);
    WSADATA WSAData;

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

    // get a new listening socket;
    int TCPServer = socketp(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(TCPServer == INVALID_SOCKET) {
        setTextColor(RED_TXT);
        printf("Failed to initialize TCP server socket, error: %d\n", WSAGetLastErrorp());
        resetColor();
        WSACleanupp();
        return 1;
    } else {
        setTextColor(GREEN_TXT);
        printf("TCP server socket successfully initialized\n");
        resetColor();
    }

    struct sockaddr_in localAddr;
    memset(&localAddr, 0, sizeof(localAddr));
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = INADDR_ANY;
    localAddr.sin_port = htonsp(55555);

    // bind the listening socket;
    int bindRes = bindp(TCPServer, (struct sockaddr*)(&localAddr), sizeof(localAddr));
    if(bindRes == SOCKET_ERROR) {
        setTextColor(RED_TXT);
        printf("Binding error: %d\n", WSAGetLastErrorp());
        resetColor();
        WSACleanupp();
        return 1;
    } else {
        setTextColor(GREEN_TXT);
        printf("Completed binding server socket\n");
        resetColor();
    }

    // and ready to listen ;
    listenp(TCPServer, 10);

    ptrMutex = CreateMutex(NULL, FALSE, NULL);
    int flag = 0;
    while(1) {
        // unable to accpet more connection;
        if(head >= queSize) {
            Sleep(10);
            continue;
        } else if(!head) {
            if(flag) break;
            flag = 1;
        }
        printf("\n");

        // use the head node;
        int now = head, 
            addrLen = sizeof(connVec[now].sourceAddr);
        connVec[now].index = now;
        connVec[now].TCPClient = acceptp(TCPServer, 
                                        (struct sockaddr*)(&connVec[now].sourceAddr), 
                                        (int *)(&addrLen));

        WaitForSingleObject(ptrMutex, INFINITE);
        if(connVec[now].TCPClient == INVALID_SOCKET) {
            setTextColor(RED_TXT);
            printf("Failed to establish connection with client, error: %d\n", WSAGetLastErrorp());
            resetColor();
            
            // back to empty status;
            connVec[now].TCPClient = 0;
            continue;
        } else {
            char enterBuffer[bufSize+1];
            // set new head and tail;
            getHead(&head);
            tail = Max(tail, now);
            sprintf(enterBuffer, 
                    "\nClient with:\nIP %s\nport %d\nhas enter the chat room\n", 
                    inet_ntoap(connVec[now].sourceAddr.sin_addr), 
                    ntohsp(connVec[now].sourceAddr.sin_port));

            setTextColor(GREEN_TXT);
            printf("%s", enterBuffer);
            resetColor();
            sendAll(&connVec[now], enterBuffer);
        }
        ReleaseMutex(ptrMutex);

        // create a thread to handle the connection;
        connVec[now].recvThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)recvAndSend, &connVec[now], 0, NULL);
    }

    terminateFlag = 1;
    closesocketp(TCPServer);
    WSACleanupp();

    return 0;
}


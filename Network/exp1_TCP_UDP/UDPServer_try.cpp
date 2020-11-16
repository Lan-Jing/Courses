#include <iostream>
using namespace std;

#include <windows.h>
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <time.h>
#include <conio.h>

// linking lib file for winsock.
#pragma comment(lib,"ws2_32.lib");

typedef int __stdcall (*WSAStartup_p) (WORD, LPWSADATA);
typedef SOCKET __stdcall (*socket_p) (int, int, int);
typedef int __stdcall (*closesocket_p) (SOCKET);
typedef int __stdcall (*WSAGetLastError_p) ();
typedef int __stdcall (*WSACleanup_p) ();
typedef int __stdcall (*bind_p) (SOCKET, const sockaddr *, int);
typedef int __stdcall (*recvfrom_p) (SOCKET, char *, int, int, sockaddr *, int *);
typedef u_short __stdcall (*htons_p) (u_short);
typedef unsigned long __stdcall (*inet_addr_p) (const char *);

const int bufSize = 2000;

int main(int argc, char *argv[]) {
    HMODULE hDll = LoadLibrary("ws2_32.dll");
    if(hDll == NULL) {
        cout<<"fail to load Winsock Dll"<<endl;
        system("pause");
        return 1;
    }
    WSAStartup_p WSAStartup = (WSAStartup_p)GetProcAddress(hDll,"WSAStartup");
    socket_p socket = (socket_p)GetProcAddress(hDll,"socket");
    closesocket_p closesocket = (closesocket_p)GetProcAddress(hDll,"closesocket");
    WSAGetLastError_p WSAGetLastError = (WSAGetLastError_p)GetProcAddress(hDll,"WSAGetLastError");
    WSACleanup_p WSACleanup = (WSACleanup_p)GetProcAddress(hDll,"WSACleanup");
    bind_p bind = (bind_p)GetProcAddress(hDll,"bind");
    recvfrom_p recvfrom = (recvfrom_p)GetProcAddress(hDll,"recvfrom");
    htons_p htons = (htons_p)GetProcAddress(hDll,"htons");
    inet_addr_p inet_addr = (inet_addr_p)GetProcAddress(hDll,"inet_addr");


    WORD WSAVersionRequest = MAKEWORD(2,2);
    WSADATA WSAData;
    int startupError;

    //try booting Winsock.
    startupError = WSAStartup(WSAVersionRequest, &WSAData);
    if(startupError) {
        cout<<"Startup failed with error: "<<startupError<<endl;
        system("pause");
        return 1;
    } else {
        cout<<"Startup complete for version "<<(int)HIBYTE(WSAData.wVersion)<<'.'<<(int)LOBYTE(WSAData.wVersion)<<endl;
    }

    SOCKET UDPSockServer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(UDPSockServer != INVALID_SOCKET) {
        cout<<"Local socket initialized"<<endl;
    } else {
        cout<<"Local socket failed to initialize with error: "<<WSAGetLastError()<<endl;
        system("pause");
        WSACleanup();
        return 1;
    }

    struct sockaddr_in serverAdd;
    memset(&serverAdd,0,sizeof(serverAdd));
    int bindError;
    const short host = 55555;

    serverAdd.sin_family = AF_INET;
    // listen to any port.
    serverAdd.sin_addr.S_un.S_addr = INADDR_ANY;
    serverAdd.sin_port = htons(host);
    bindError = bind(UDPSockServer, (sockaddr*)(&serverAdd), sizeof(serverAdd));
    if(bindError) {
        cout<<"Binding failed with error: "<<WSAGetLastError()<<endl;
        WSACleanup();
        system("pause");
        return 1;
    } else {
        cout<<"Binding succeeded"<<endl<<endl;
    }

    char buffer[bufSize+1];
    while(!kbhit()) {
        struct sockaddr senderAdd;
        int senderAddSize = sizeof(senderAdd);
        int recvRes;
        cout<<endl<<"Receiving message..."<<endl;
        recvRes = recvfrom(UDPSockServer, buffer, bufSize, 0, &senderAdd, &senderAddSize);
        if(recvRes == SOCKET_ERROR) {
            cout<<"Receiving message failed with error:"<<WSAGetLastError()<<endl;
            break;
        } else {
            if(!recvRes) continue;
            buffer[recvRes] = '\0';
            cout<<"Message received:"<<endl<<buffer<<endl;
        }
    }

    closesocket(UDPSockServer);

    WSACleanup();

    system("pause");
    return 0;
}
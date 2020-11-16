#include <iostream>
using namespace std;

#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <time.h>
#include <string>
#include <cstring>

#pragma comment(lib,"ws2_32.lib");

typedef int __stdcall (*WSAStartup_p) (WORD, LPWSADATA);
typedef SOCKET __stdcall (*socket_p) (int, int, int);
typedef int __stdcall (*closesocket_p) (SOCKET);
typedef int __stdcall (*WSAGetLastError_p) ();
typedef int __stdcall (*WSACleanup_p) ();
typedef int __stdcall (*bind_p) (SOCKET, const sockaddr *, int);
typedef int __stdcall (*sendto_p) (SOCKET, const char *, int, int, const sockaddr *, int);
typedef int __stdcall (*recvfrom_p) (SOCKET, char *, int, int, sockaddr *, int *);
typedef u_short __stdcall (*htons_p) (u_short);
typedef unsigned long __stdcall (*inet_addr_p) (const char *);

const int bufSize = 2000;

int main() {
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
    sendto_p sendto = (sendto_p)GetProcAddress(hDll,"sendto");
    recvfrom_p recvfrom = (recvfrom_p)GetProcAddress(hDll,"recvfrom");
    htons_p htons = (htons_p)GetProcAddress(hDll,"htons");
    inet_addr_p inet_addr = (inet_addr_p)GetProcAddress(hDll,"inet_addr");

    //open up Winsock.
    WORD WSVersionRequest = MAKEWORD(2,2);
    WSADATA WSAData;
    int startupRes;

    startupRes = WSAStartup(WSVersionRequest, &WSAData);
    if(startupRes) {
        cout<<"Startup failed with error: "<<startupRes<<endl;
        system("pause");
        return 1;
    } else {
        cout<<"Startup completed for Winsock "<<(int)HIBYTE(WSAData.wVersion)<<'.'<<(int)LOBYTE(WSAData.wVersion)<<endl;
    }

    // initialize an available UDP socket.
    SOCKET UDPClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(UDPClient == INVALID_SOCKET) {
        cout<<"fail to initialize socket with error: "<<WSAGetLastError()<<endl;
        WSACleanup();
        system("pause");
        return 1; 
    } else {
        cout<<"UDP client socket initialized"<<endl;
    }

    // set information for the receiver.
    const char* IPAddr = "127.0.0.1";
    const short port = 55555;

    struct sockaddr_in targetAddr;
    targetAddr.sin_family = AF_INET;
    targetAddr.sin_addr.S_un.S_addr = inet_addr(IPAddr);
    targetAddr.sin_port = htons(port);

    // get message.
    char buffer[bufSize+1];
    while(1) {
        memset(buffer,0,sizeof(buffer));
        printf("Enter Message, '0' to end communication:");
        scanf("%s",&buffer);
        cout<<endl;

        int sendRes;
        sendRes = sendto(UDPClient, buffer, strlen(buffer), 0, (sockaddr*)(&targetAddr), sizeof(targetAddr));
        if(sendRes == SOCKET_ERROR) {
            cout<<"Message failed to be sent with error: "<<WSAGetLastError()<<endl;
            WSACleanup();
            system("pause");
            return 1; 
        } else {
            cout<<"Message:"<<endl<<buffer<<endl<<"successfully sent"<<endl<<endl;
        }
        
        if(buffer[0] == '0' && strlen(buffer) == 1) 
            break;

        memset(buffer,0,sizeof(buffer));
        printf("Receiving message from target address...\n");
        
        int targetAddrSize = sizeof(targetAddr);
        int recvRes;
        recvRes = recvfrom(UDPClient, buffer, bufSize, 0, (sockaddr*)(&targetAddr), &targetAddrSize);
        if(recvRes == SOCKET_ERROR) {
            cout<<"Fail to receive message with error: "<<WSAGetLastError()<<endl;
            WSACleanup();
            system("pause");
            return 1; 
        } else {
            cout<<buffer;
        }
    }

    closesocket(UDPClient);

    WSACleanup();

    system("pause");
    return 0;

}
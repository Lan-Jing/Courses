#include <iostream>
using namespace std;

#include <windows.h>
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <time.h>

// linking lib file for winsock.
#pragma comment(lib,"ws2_32.lib");

typedef int __stdcall (*WSAStartup_p) (WORD, LPWSADATA);
typedef SOCKET __stdcall (*socket_p) (int, int, int);
typedef int __stdcall (*sendto_p) (SOCKET, const char *, int, int, const sockaddr *, int);
typedef int __stdcall (*closesocket_p) (SOCKET);
typedef int __stdcall (*WSAGetLastError_p) ();
typedef int __stdcall (*WSACleanup_p) ();
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
    sendto_p sendto = (sendto_p)GetProcAddress(hDll,"sendto");
    closesocket_p closesocket = (closesocket_p)GetProcAddress(hDll,"closesocket");
    WSAGetLastError_p WSAGetLastError = (WSAGetLastError_p)GetProcAddress(hDll,"WSAGetLastError");
    WSACleanup_p WSACleanup = (WSACleanup_p)GetProcAddress(hDll,"WSACleanup");
    htons_p htons = (htons_p)GetProcAddress(hDll,"htons");
    inet_addr_p inet_addr = (inet_addr_p)GetProcAddress(hDll,"inet_addr");

    // parameter for starting up Winsock API.
    WORD WSVersionRequest = MAKEWORD(2,2);
    WSADATA WSAData;
    int startupError;

    // try booting Winsock and check result.
    startupError = WSAStartup(WSVersionRequest,&WSAData);
    if(startupError) {
        cout<<"Startup failed with error code: "<<startupError<<endl;
        system("pause");
        return 1;
    } else {
        cout<<"Startup complete for version "<<(int)HIBYTE(WSAData.wVersion)<<'.'<<(int)LOBYTE(WSAData.wVersion)<<endl;
    }
    
    // initialize local socket for UDP applications.
    // af : address family 
    // type : socket data type, DGRAM for UDP
    // protocol : UDP, TCP...
    SOCKET UDPSockClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(UDPSockClient != INVALID_SOCKET) {
        cout<<"Local socket initialized successfully"<<endl<<endl;
    } else {
        cout<<"Local socket failed to initialize with error: "<<WSAGetLastError()<<endl;
        WSACleanup();
        system("pause");
        return 1;
    }

    // specify information for receiver.
    const char *addr = "127.0.0.1";
    const short host = 55555;

    struct sockaddr_in targetAddr;
    targetAddr.sin_family = AF_INET;
    targetAddr.sin_addr.s_addr = inet_addr(addr);
    targetAddr.sin_port = htons(host);

    // write in message.
    char buffer[bufSize+1];
    const char *message = "the_first_message";
    memcpy(buffer,message,strlen(message));
    buffer[strlen(message)] = '\0';

    // send the message to server specified by targetAddr.
    int sendError;
    sendError = sendto(UDPSockClient, buffer, strlen(message), 0, 
                       (sockaddr*)(&targetAddr), sizeof(targetAddr));
    if(sendError == SOCKET_ERROR) {
        cout<<"Message failed to send with error: "<<WSAGetLastError()<<endl;
        WSACleanup();
        system("pause");
        return 1;
    } else {
        cout<<"Message: "<<endl<<buffer<<endl<<"successfully sent"<<endl;
    }

    // close the local socket.
    closesocket(UDPSockClient);

    // unload Winsock dll.
    WSACleanup();

    system("pause");
    return 0;
}

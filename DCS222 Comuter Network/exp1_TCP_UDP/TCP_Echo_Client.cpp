#include <iostream>
using namespace std;

#include <winsock2.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <ctime>
#include <time.h>

#pragma comment(lib,"ws2_32.lib");

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
    htons_p htons = (htons_p)GetProcAddress(hDll,"htons");
    inet_addr_p inet_addr = (inet_addr_p)GetProcAddress(hDll,"inet_addr");
    connect_p connect = (connect_p)GetProcAddress(hDll,"connect");
    send_p send = (send_p)GetProcAddress(hDll,"send");
    recv_p recv = (recv_p)GetProcAddress(hDll,"recv");

    WORD WSVersionRequest = MAKEWORD(2,2);
    WSADATA WSAData;
    int startRes;

    startRes = WSAStartup(WSVersionRequest, &WSAData);
    if(startRes) {
        cout<<"Winsock startup failed with error: "<<startRes<<endl;
        system("pause");
        return 1;
    } else {
        cout<<"Winsock startup complete, version: "<<HIBYTE(WSAData.wVersion)<<'.'<<LOBYTE(WSAData.wVersion)<<endl;
    }

    struct sockaddr_in targetAddr;
    memset(&targetAddr, 0, sizeof(targetAddr));
    targetAddr.sin_family = AF_INET;
    targetAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    targetAddr.sin_port = htons(55555);

    while(1) {
        cout<<endl;
        int TCPClient;
        TCPClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(TCPClient == INVALID_SOCKET) {
            cout<<"Failed to initialize TCP client socket, error: "<<WSAGetLastError()<<endl;
            WSACleanup();
            system("pause");
            return 1;
        } else {
            cout<<"TCP client socket successfully initialized"<<endl;
        }
        
        int connectRes = connect(TCPClient, (sockaddr*)(&targetAddr), sizeof(targetAddr));
        if(connectRes == SOCKET_ERROR) {
            cout<<"Failed to connect to the TCP server, error: "<<WSAGetLastError()<<endl;
            WSACleanup();
            system("pause");
            return 1;
        } else {
            cout<<"Connection completed"<<endl;
        }

        char buffer[bufSize+1];
        memset(buffer,0,sizeof(buffer));

        cout<<"Enter message to Server, 0 to quit:";
        scanf("%s",buffer);
        cout<<"Sending message..."<<endl;

        int sendRes = send(TCPClient, buffer, strlen(buffer), 0);
        if(sendRes == SOCKET_ERROR) {
            cout<<"Failed to send message, error: "<<WSAGetLastError()<<endl;
            WSACleanup();
            system("pause");
            return 1;
        } else {
            cout<<"Message successfully sent to server"<<endl<<endl;
        }

        memset(buffer,0,sizeof(buffer));
        cout<<"Receiving message from server..."<<endl;
        int recvRes = recv(TCPClient, buffer, bufSize, 0);
        if(recvRes == SOCKET_ERROR) {
            cout<<"Failed to receive message from server, error: "<<WSAGetLastError()<<endl;
            WSACleanup();
            system("pause");
            return 1;
        } else if(recvRes) {
            cout<<buffer<<endl;
        } else {
            cout<<"connection closed"<<endl;
            break;
        }

        closesocket(TCPClient);
    }    
    WSACleanup();

    system("pause");
    return 0;
}
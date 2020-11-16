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
typedef int __stdcall (*bind_p) (SOCKET, const sockaddr *, int);
typedef u_short __stdcall (*htons_p) (u_short);
typedef unsigned long __stdcall (*inet_addr_p) (const char *);
typedef int __stdcall (*connect_p) (SOCKET, const sockaddr *, int);
typedef int __stdcall (*send_p) (SOCKET, const char *, int, int);
typedef int __stdcall (*recv_p) (SOCKET, char *, int, int);
typedef int __stdcall (*listen_p) (SOCKET, int);
typedef SOCKET __stdcall (*accept_p) (SOCKET, sockaddr *, int *);

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
    htons_p htons = (htons_p)GetProcAddress(hDll,"htons");
    connect_p connect = (connect_p)GetProcAddress(hDll,"connect");
    send_p send = (send_p)GetProcAddress(hDll,"send");
    recv_p recv = (recv_p)GetProcAddress(hDll,"recv");
    listen_p listen = (listen_p)GetProcAddress(hDll,"listen");
    accept_p accept = (accept_p)GetProcAddress(hDll,"accept");

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

    int TCPServer; 
    TCPServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(TCPServer == INVALID_SOCKET) {
        cout<<"Failed to initialize TCP server socket, error: "<<WSAGetLastError()<<endl;
        WSACleanup();
        system("pause");
        return 1;
    } else {
        cout<<"TCP server socket successfully initialized"<<endl;
    }

    struct sockaddr_in localAddr;
    memset(&localAddr, 0, sizeof(localAddr));
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = INADDR_ANY;
    localAddr.sin_port = htons(55555);

    int bindRes;
    bindRes = bind(TCPServer, (struct sockaddr*)(&localAddr), sizeof(localAddr));
    if(bindRes == SOCKET_ERROR) {
        cout<<"Binding error: "<<WSAGetLastError()<<endl;
        WSACleanup();
        system("pause");
        return 1;
    } else {
        cout<<"Completed binding server socket"<<endl;
    }

    listen(TCPServer, 5);

    while(1) {
        cout<<endl;

        struct sockaddr sourceAddr;
        int sourceAddrSize = sizeof(sourceAddr);
        int TCPConnect = accept(TCPServer, &sourceAddr, &sourceAddrSize);
        if(TCPConnect == INVALID_SOCKET) {
            cout<<"Failed to establish connection with client, error: "<<WSAGetLastError()<<endl;
            WSACleanup();
            system("pause");
            return 1;
        } else {
            cout<<"Connection established"<<endl;
        }

        char buffer[bufSize+1];
        memset(buffer,0,sizeof(buffer));
        int recvRes;

        recvRes = recv(TCPConnect, buffer, bufSize, 0);
        if(recvRes == SOCKET_ERROR) {
            cout<<"Failed to receive message, error: "<<WSAGetLastError()<<endl;
            WSACleanup();
            system("pause");
            return 1;
        } else {
            cout<<"Message Received:"<<buffer<<endl;
        }

        time_t serverTime;
        time(&serverTime);
        cout<<"Time: "<<ctime(&serverTime)<<endl;

        if(buffer[0] == '0' && strlen(buffer) == 1) {
            closesocket(TCPConnect);
            break;
        }

        cout<<"Sending back message..."<<endl;
        string message = "";
        message += "Message:" + string(ctime(&serverTime), strlen(ctime(&serverTime)));
        message += "Time:" + string(buffer, strlen(buffer)) + "\n";
        
        int sendRes;
        sendRes = send(TCPConnect, message.c_str(), strlen(message.c_str()), 0);
        if(sendRes == SOCKET_ERROR) {
            cout<<"Failed to send message back to client, error: "<<WSAGetLastError()<<endl;
            WSACleanup();
            system("pause");
            return 1;
        } else {
            cout<<"Message successfully sent back"<<endl;
        }

        closesocket(TCPConnect);
    }

    closesocket(TCPServer);

    WSACleanup();

    system("pause");
    return 0;
}
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
typedef u_short __stdcall (*ntohs_p) (u_short);
typedef char *__stdcall (*inet_ntoa_p) (in_addr);
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
    ntohs_p ntohs = (ntohs_p)GetProcAddress(hDll,"ntohs");
    inet_ntoa_p inet_ntoa = (inet_ntoa_p)GetProcAddress(hDll,"inet_ntoa");
    inet_addr_p inet_addr = (inet_addr_p)GetProcAddress(hDll,"inet_addr");

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

    SOCKET UDPServer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(UDPServer == INVALID_SOCKET) {
        cout<<"Failed to initialize socket with error: "<<WSAGetLastError()<<endl;
        WSACleanup();
        system("pause");
        return 1;
    } else {
        cout<<"UDP server socket initialized"<<endl;
    }

    const short port = 55555;
    struct sockaddr_in localAddr;
    struct sockaddr_in senderAddr;
    int senderAddrSize = sizeof(senderAddr);
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.S_un.S_addr = INADDR_ANY;
    localAddr.sin_port = htons(port);
    
    int bindRes;
    bindRes = bind(UDPServer, (sockaddr*)(&localAddr), sizeof(localAddr));
    if(bindRes == SOCKET_ERROR) {
        cout<<"Binding local socket failed with error: "<<WSAGetLastError()<<endl;
        WSACleanup();
        system("pause");
        return 1;
    } else {
        cout<<"Binding local socket completed"<<endl;
    }

    char buffer[bufSize+1];
    int recvRes;
    while(1) {
        cout<<endl<<"Receiving message..."<<endl;
        memset(buffer,0,sizeof(buffer));
        recvRes = recvfrom(UDPServer, buffer, bufSize, 0, (sockaddr*)(&senderAddr), &senderAddrSize);
        if(recvRes == SOCKET_ERROR) {
            cout<<"Receiving message from client failed with error: "<<WSAGetLastError()<<endl;
            WSACleanup();
            system("pause");
            return 1;
        } else {
            cout<<"Message received:"<<endl<<buffer<<endl;
            cout<<"Client IP address:"<<inet_ntoa(senderAddr.sin_addr)<<endl;
            cout<<"Client port number:"<<ntohs(senderAddr.sin_port)<<endl;
        }

        time_t recvTime;
        time(&recvTime);
        cout<<"Time:"<<ctime(&recvTime)<<endl;

        if(buffer[0] == '0' && strlen(buffer) == 1)
            break;

        cout<<"Sending back message..."<<endl;
        int sendRes;

        string backInfo, tmp;
        backInfo += "Message from client:";
        backInfo += string(buffer,strlen(buffer));
        backInfo += "\nClient IP address:";
        backInfo += string(inet_ntoa(senderAddr.sin_addr),strlen(inet_ntoa(senderAddr.sin_addr)));
        backInfo += "\nClient port number:";
        backInfo += to_string(ntohs(senderAddr.sin_port));
        backInfo += "\nServer time:";
        backInfo += string(ctime(&recvTime),strlen(ctime(&recvTime))) + '\n';
        memcpy(buffer, backInfo.c_str(), backInfo.length());

        sendRes = sendto(UDPServer, buffer, strlen(buffer), 0, (sockaddr*)(&senderAddr), sizeof(senderAddr));
        if(sendRes == SOCKET_ERROR) {
            cout<<"Sending back message failed with error: "<<WSAGetLastError()<<endl;
            WSACleanup();
            system("pause");
            return 1;
        } else {
            cout<<"Successfully sent message back to client."<<endl<<endl;
        }
    }

    closesocket(UDPServer);

    WSACleanup();

    system("pause");
    return 0;
}
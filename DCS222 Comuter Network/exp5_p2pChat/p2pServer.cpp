#include "fileStruct.h"

char *hostname, *port;
int sock;

extern int terminateFlag;

int main() {
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
    // create the local socket;
    int TCPServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(TCPServer == INVALID_SOCKET) {
        printf("Failed to setup TCP server, error:%d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    } else
        printf("Socket setup successfully\n");

    // set local information;
    struct sockaddr_in localAddr;
    memset(&localAddr, 0, sizeof(localAddr));
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = INADDR_ANY;
    localAddr.sin_port = htons(55555);

    // bind the listening socket;
    int bindRes = bind(TCPServer, (struct sockaddr *)(&localAddr), sizeof(localAddr));
    if(bindRes == SOCKET_ERROR) {
        printf("Binding error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    } else
        printf("Successfully binded\n");
    // and ready to listen ;
    listen(TCPServer, 10);

    struct sockaddr sourceAddr;
    int sourceAddrSize = sizeof(sourceAddr);
    int sock = accept(TCPServer, &sourceAddr, &sourceAddrSize);
    if(sock == INVALID_SOCKET) {
        printf("Failed to establish connection with client, error:%d\n", WSAGetLastError());
        WSACleanup();
        closesocket(TCPServer);
        return 1;
    } else {
        printf("Connection established\n");
    }

    HANDLE recvThread, sendThread;
    sendThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)sendBuffer, &sock, 0, NULL);
    recvThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)recvData, &sock, 0, NULL);

    WaitForSingleObject(sendThread, INFINITE);
    WaitForSingleObject(recvThread, INFINITE);
    CloseHandle(recvThread);
    CloseHandle(sendThread);

    terminateFlag = 1;
    closesocket(TCPServer);
    closesocket(sock);
    WSACleanup();
    return 0;
}
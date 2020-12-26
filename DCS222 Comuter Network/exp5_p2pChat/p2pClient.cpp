#include "fileStruct.h"

char *hostname, *port;
int sock;

extern int terminateFlag;

int main(int argc, char *argv[]) {
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

    //hostname = argv[1], port = argv[2];
    hostname = (char *)"127.0.0.1", port = (char *)"55555";
    sock = connectSock(hostname, port);

    HANDLE recvThread, sendThread;
    sendThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)sendBuffer, &sock, 0, NULL);
    recvThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)recvData, &sock, 0, NULL);

    WaitForSingleObject(sendThread, INFINITE);
    WaitForSingleObject(recvThread, INFINITE);
    CloseHandle(recvThread);
    CloseHandle(sendThread);

    terminateFlag = 1;
    closesocket(sock);
    WSACleanup();
    return 0;
}
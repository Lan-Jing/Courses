import grpc
import md_server_pb2 as ms
import md_server_pb2_grpc as ms_grpc

from concurrent import futures
import time
import threading

import ms_conf

# count in each second, remove servers that aren't responding.
def timeout(targetList, lock, idSet):
    while True:
        time.sleep(1)
        
        rmList = []
        lock.acquire()
        for inode in targetList.keys():
            targetList[inode][1] -= 1
            if targetList[inode][1] <= 0:
                rmList.append(inode)
        for inode in rmList:
            idSet.add(targetList[inode][0])
            targetList.pop(inode, 0)
            ip, port = inode.strip().split()
            print("File server {}:{} is offline.".format(ip, port))
        lock.release()

class md_server(ms_grpc.md_serverServicer):
    def __init__(self):
        self.ip = ms_conf.md_serverIP
        self.port = ms_conf.md_serverPort
        self.timeout = ms_conf.md_timeout

        self.serverIdSet = set(range(1,64))
        self.clientIdSet = set(range(1,64))
        self.targetList = {}
        self.listLock = threading.Lock()
        timeoutThread = threading.Thread(target=timeout, args=(self.targetList, self.listLock, self.serverIdSet))
        timeoutThread.start()

        print("The metadata server is available on {}:{}".format(self.ip, self.port))

    # assign unique id to clients
    def connect(self, request, context):
        newId = self.clientIdSet.pop()
        return ms.ms_reply(done=newId)

    # The client returns id
    def disconnect(self, request, context):
        self.clientIdSet.add(request.done)
        return ms.ms_reply(done=1)

    # insert or update a given file server
    def online(self, request, context):
        server = request.ip + " " + request.port
        if server not in self.targetList:
            newId = self.serverIdSet.pop()
            print("File server {} is on {}:{}.".format(newId, request.ip, request.port))
        else:
            newId = self.targetList[server][0]

        self.listLock.acquire()
        self.targetList[server] = [newId, self.timeout]
        self.listLock.release()

        return ms.ms_reply(done=newId)

    # remove a server from the list, free its id
    def offline(self, request, context):
        server = request.ip + " " + request.port

        self.listLock.acquire()
        self.serverIdSet.add(self.targetList[server][0])
        self.targetList.pop(server, 0)
        self.listLock.release()

        print("File server {}:{} is offline.".format(request.ip, request.port))
        return ms.ms_reply(done=1)

    # list all online servers
    def listServer(self, request, context):
        serverList = ms.serverInfoList(done=1)
        
        self.listLock.acquire()
        for inode in self.targetList.keys():
            ip, port = inode.strip().split()
            serverList.sList.append(ms.serverInfo(ip=ip, port=port, id=self.targetList[inode][0]))
        self.listLock.release()
        
        return serverList

def serve():
    servicer = md_server()
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    ms_grpc.add_md_serverServicer_to_server(servicer, server)
    server.add_insecure_port("[::]:{}".format(servicer.port))
    server.start()
    server.wait_for_termination()

if __name__ == "__main__":
    serve()
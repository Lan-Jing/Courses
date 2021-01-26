import sys
sys.path.insert(0, "../md_server")

import grpc
import file_server_pb2 as fs
import file_server_pb2_grpc as fs_grpc
import md_server_pb2 as ms
import md_server_pb2_grpc as ms_grpc
import ms_conf

from concurrent import futures
import time
import threading

import os
import os.path as ospath

# send heartbeat to the metadata server
def heartbeat(stub, ip, port):
    while True:
        time.sleep(2)
        stub.online(ms.serverInfo(ip=ip, port=port))

class file_server(fs_grpc.file_serverServicer):
    def __init__(self, ip, port, rootPath):
        # contact the metadata server
        mdChannel = grpc.insecure_channel(ms_conf.md_serverIP + ":" + ms_conf.md_serverPort)
        self.mdStub = ms_grpc.md_serverStub(mdChannel)

        self.ip = ip
        self.port = port
        self.online()

        self.rootPath = rootPath + "/fserver_{}".format(self.id)
        if not ospath.exists(self.rootPath):
            os.mkdir(self.rootPath)
        os.chdir(self.rootPath)
        print("Root of the server: {}".format(self.rootPath))

        self.curPathDict = {}

        # fork thread to maintain heartbeat
        heartbeatThread = threading.Thread(target=heartbeat, args=(self.mdStub, self.ip, self.port))
        heartbeatThread.start()

    # register on the metadata server to be visible to clients
    def online(self):
        self.id = (self.mdStub.online(ms.serverInfo(ip=self.ip, port=self.port))).done
        print("File server {} is available on {}:{}".format(self.id, self.ip, self.port))
    
    # remove self from the md server.
    def offline(self):
        self.mdStub.offline(ms.serverInfo(ip=self.ip, port=self.port))
        print("File server {} now offline\n".format(self.id))

    # return a stub connecting to another server.
    def stub_gen(self, serverList):
        for serverInfo in serverList:
            if serverInfo.id == self.id:
                continue
            channel = grpc.insecure_channel(serverInfo.ip + ":" + serverInfo.port)
            stub = fs_grpc.file_serverStub(channel)
            yield stub

    # return the absolute path on the server
    def pwd(self, request, context):
        if request.id not in self.curPathDict:
            self.curPathDict[request.id] = ""

        if request.cascading == True:
            gen = self.stub_gen((self.mdStub.listServer(ms.empty())).sList)
            while True:
                try:
                    next(gen).pwd(fs.stringMes(id=request.id, cascading=0))
                except StopIteration:
                    break
        
        return fs.stringMes(str=self.curPathDict[request.id])

    # list all items in the current dir
    def ls(self, request, context):
        path = self.rootPath + "/" + self.curPathDict[request.id]
        return fs.stringMes(str="\n".join(os.listdir(path)))

    # enter a given location. Clients are only permitted to operate inside the root dir
    def cd(self, request, context):
        path = self.rootPath + "/" + self.curPathDict[request.id] + "/" + request.str
        if request.str[0] == '/' or not ospath.exists(path) or not ospath.isdir(path):
            return fs.fs_reply(done=0)
        
        path = ospath.abspath(path)
        # dir pointed by path must be a sub-dir of the root
        if len(ospath.commonpath([path, self.rootPath])) < len(self.rootPath):
            return fs.fs_reply(done=0)
        
        self.curPathDict[request.id] = path[len(self.rootPath):]

        if request.cascading == True:
            gen = self.stub_gen((self.mdStub.listServer(ms.empty())).sList)
            while True:
                try:
                    next(gen).cd(fs.stringMes(str=request.str, id=request.id, cascading=0))
                except StopIteration:
                    break

        return fs.fs_reply(done=1)
        
    # make a new dir given a valid path
    def mkdir(self, request, context):
        path = self.rootPath + "/" + self.curPathDict[request.id] + "/" + request.str
        if request.str[0] == '/' or ospath.exists(path):
            return fs.fs_reply(done=0)

        path = ospath.abspath(path)
        if len(ospath.commonpath([path, self.rootPath])) < len(self.rootPath):
            return fs.fs_reply(done=0)

        os.mkdir(path)

        if request.cascading == True:
            gen = self.stub_gen((self.mdStub.listServer(ms.empty())).sList)
            while True:
                try:
                    next(gen).mkdir(fs.stringMes(str=request.str, id=request.id, cascading=0))
                except StopIteration:
                    break

        return fs.fs_reply(done=1)

    # remove a file or dir. Recursive delete for a dir
    def rm(self, request, context):
        path = self.rootPath + "/" + self.curPathDict[request.id] + "/" + request.str
        if request.str[0] == '/' or not ospath.exists(path):
            return fs.fs_reply(done=0)

        path = ospath.abspath(path)
        if len(ospath.commonpath([path, self.rootPath])) < len(self.rootPath):
            return fs.fs_reply(done=0)

        if ospath.isdir(path):
            os.removedirs(path)
        else:
            os.remove(path)

        if request.cascading == True:
            gen = self.stub_gen((self.mdStub.listServer(ms.empty())).sList)
            while True:
                try:
                    next(gen).rm(fs.stringMes(str=request.str, id=request.id, cascading=0))
                except StopIteration:
                    break

        return fs.fs_reply(done=1)

    # upload given file to the server. Broadcast update to other replicas
    def upload(self, request, context):
        path = self.rootPath + "/" + self.curPathDict[request.id] + "/" + request.path
        if request.path[0] == '/':
            return fs.fs_reply(done=0)

        path = ospath.abspath(path)
        if len(ospath.commonpath([path, self.rootPath])) < len(self.rootPath):
            return fs.fs_reply(done=0)

        if not ospath.exists(path):
            os.mknod(path)  
        with open(path, "wb") as fp:
            fp.write(request.buffer)

        if request.cascading == True:
            gen = self.stub_gen((self.mdStub.listServer(ms.empty())).sList)
            while True:
                try:
                    next(gen).upload(fs.upRequest(path=request.path, buffer=request.buffer, id=request.id, cascading=0))
                except StopIteration:
                    break

        return fs.fs_reply(done=1)

    # download given file from the server. 
    def download(self, request, context):
        path = self.rootPath + "/" + self.curPathDict[request.id] + "/" + request.path
        if request.path[0] == '/' or not ospath.exists(path) or not ospath.isfile(path):
            return fs.bufferMes(done=0)

        path = ospath.abspath(path)
        if len(ospath.commonpath([path, self.rootPath])) < len(self.rootPath):
            return fs.bufferMes(done=0)

        with open(path, "rb") as fp:
            content = fp.read()
            return fs.bufferMes(buffer=content, done=1)

def serve(ip, port, rootPath):
    servicer = file_server(ip, port, rootPath)
    if not ospath.exists(rootPath):
        os.mkdir(rootPath)
    
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    fs_grpc.add_file_serverServicer_to_server(servicer, server)
    server.add_insecure_port("[::]:{}".format(port))
    server.start()
    server.wait_for_termination()

if __name__ == "__main__":
    serve("127.0.0.1", sys.argv[1], "/home/jing/code/dfs/data")
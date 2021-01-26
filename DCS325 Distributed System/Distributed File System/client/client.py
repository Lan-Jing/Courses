import sys
sys.path.insert(0, "../file_server")
sys.path.insert(0, "../md_server")
sys.path.insert(0, "../lock_manager")

import grpc
import file_server_pb2 as fs
import file_server_pb2_grpc as fs_grpc
import md_server_pb2 as ms
import md_server_pb2_grpc as ms_grpc
import ms_conf
import lock_manager_pb2 as lm
import lock_manager_pb2_grpc as lm_grpc
import lm_conf

import os
import os.path as ospath
import random

class Client():
    def __init__(self, rootPath):
        self.lockStub = self.connect_lock_manager()
        self.mdStub = self.connect_md_server()
        self.id = (self.mdStub.connect(ms.empty())).done
        self.openFileList = {}
        self.fileIdSet = set(range(1,65536))

        # create client directory
        self.curPath = rootPath + "/client_{}".format(self.id)
        if not ospath.exists(self.curPath):
            os.mkdir(self.curPath)

        print("Client {} is now online.\n".format(self.id))
        self.fileStub = self.connect_file_server()
    
    # connect to the lock manager
    def connect_lock_manager(self):
        lock_channel = grpc.insecure_channel(lm_conf.lockManagerIP + ":" + lm_conf.lockManagerPort)
        print("Connecting to the lock manager on {}:{}\n".format(lm_conf.lockManagerIP, lm_conf.lockManagerPort))
        return lm_grpc.lock_managerStub(lock_channel)

    # connect to the metadata server
    def connect_md_server(self):
        md_channel = grpc.insecure_channel(ms_conf.md_serverIP + ":" + ms_conf.md_serverPort)
        print("Connecting to the metadata server on {}:{}\n".format(ms_conf.md_serverIP, ms_conf.md_serverPort))
        return ms_grpc.md_serverStub(md_channel)

    # connect to an available file server. This will assign an unique id back to client.
    def connect_file_server(self):
        serverList = self.mdStub.listServer(ms.empty())
        print("Possible file server, first available will be choosen:\n")
        print("{:^10}{:^10}{:^10}".format("Server ID", "IP", "Port"))
        for serverInfo in serverList.sList:
            print("{:^10}{:^10}{:^10}".format(serverInfo.id, serverInfo.ip, serverInfo.port))
        
        for serverInfo in serverList.sList:
            print("Connecting to server {}...\n".format(serverInfo.id))
            fs_channel = grpc.insecure_channel(serverInfo.ip + ":" + serverInfo.port)
            self.fileStub = fs_grpc.file_serverStub(fs_channel)
            break
        
        print("Connected. Your remote pwd: {}\n".format(self.pwd(0)))
        return self.fileStub

    # output current remote path
    def pwd(self, printFlag):
        reply = self.fileStub.pwd(fs.stringMes(id=self.id, cascading=True))
        if printFlag == True:
            print(reply.str)
        return reply.str

    # list files/dirs under pwd
    def ls(self):
        reply = self.fileStub.ls(fs.id(id=self.id))
        print(reply.str)

    # enter a given path
    def cd(self, path):
        reply = self.fileStub.cd(fs.stringMes(str=path, id=self.id, cascading=True))
        if reply.done == False:
            print("{} is not a valid path in remote".format(path))
        self.pwd(1)

    # create a new dir in the remote
    def mkdir(self, path):
        reply = self.fileStub.mkdir(fs.stringMes(str=path, id=self.id, cascading=True))
        if reply.done == False:
            print("{} is not a valid path in remote".format(path))
    
    # remove a file/dir pointed by path
    def rm(self, path):
        reply = self.fileStub.rm(fs.stringMes(str=path, id=self.id, cascading=True))
        if reply.done == False:
            print("{} is not a valid path in remote".format(path))

    # lock up a given file
    def lock(self, path, lockType):
        reply = self.lockStub.lock(lm.fileInfo(filePath=path, lockType=lockType))
        if reply.done == False:
            print("Failed to lock {}".format(path))
            return False
        return True

    # unlock the file
    def unlock(self, path):
        reply = self.lockStub.unlock(lm.fileInfo(filePath=path))
        if reply.done == False:
            print("Failed to unlock {}".format(path))
            return False
        return True

    # upload a local file to the remote
    def upload(self, localPath, remotePath, openFlag):
        tmpPath = self.curPath + "/" + localPath
        for path in [localPath, tmpPath]:
            if path[0] != '/':
                continue
            path = ospath.abspath(path)
            if not ospath.exists(path) or not ospath.isfile(path):
                continue
            if len(ospath.commonpath([path, self.curPath])) < len(self.curPath):
                continue
            
            # openFlag == 1 then the file is already on lock
            fullPath = self.pwd(0)+"/"+remotePath
            if openFlag == 0 and self.lock(fullPath, 1) == False:
                return False
            
            with open(path, "rb") as fp:
                buffer = fp.read()
                reply = self.fileStub.upload(fs.upRequest(path=remotePath, buffer=buffer, id=self.id, cascading=1))

            self.unlock(fullPath)
            if reply.done == False:
                print("Failed to upload file {}. This is probably caused by invalid remote path.".format(fullPath))
                return False
            else:
                print("File {} uploaded.".format(fullPath))
                return True

        print("Invalid local file {}".format(localPath))
        return False

    # download a remote file to the local
    # openFlag: 0: download, 1: read, 2: write
    def download(self, localPath, remotePath, openFlag):
        tmpPath = self.curPath + "/" + localPath
        for path in [localPath, tmpPath]:
            if path[0] != '/':
                continue
            path = ospath.abspath(path)
            if len(ospath.commonpath([path, self.curPath])) < len(self.curPath):
                continue
            if not ospath.exists(path):
                os.mknod(path)

            fullPath = self.pwd(0)+"/"+remotePath
            if self.lock(fullPath, max(openFlag-1, 0)) == False:
                os.remove(path)
                return False

            bufferMes = self.fileStub.download(fs.downRequest(path=remotePath, id=self.id))
            if bufferMes.done == False:
                print("Failed to download file {}. This is probably caused by invalid remote path.".format(fullPath))
                os.remove(path)
                self.unlock(fullPath)
                return False
            
            with open(path, "wb") as fp:
                fp.write(bufferMes.buffer)
            print("File {} downloaded.".format(fullPath))

            if openFlag == False:
                self.unlock(fullPath)
            return path

        print("Invalid local path {}".format(localPath))
        return False

    # open a remote file. This will lock up the file and download it to local
    # mode: 0: read, 1: write
    def open(self, localPath, remotePath, mode):
        res = self.download(localPath, remotePath, mode+1)
        if res == False:
            return False
        else:
            newId = self.fileIdSet.pop()
            self.openFileList[newId] = (res, self.pwd(0)+"/"+remotePath, mode)
            return True

    # close the opened file.
    def close(self, id_str):
        id = int(id_str)
        localPath, fullPath, mode = self.openFileList[id]
        # only update for write mode
        if mode == True:
            remotePath = fullPath[len(self.pwd(0))+1:]
            res = self.upload(localPath, remotePath, mode)
            if res == False:
                return False
        else:
            self.unlock(fullPath)
        
        self.openFileList.pop(id)
        self.fileIdSet.add(id)
        return True

    # list all opened file.
    def listopen(self):
        print("{:^10}{:^50}{:^10}".format("File ID", "File Path", "Open Mode"))
        for key in self.openFileList.keys():
            print("{:^10}{:^50}{:^10}".format(key, self.openFileList[key][0], "write" if self.openFileList[key][2] else "read"))

    # close all files and return the client id back to the metadata server
    def exit(self):
        self.mdStub.disconnect(ms.ms_reply(done=self.id))
        rmList = []
        for item in self.openFileList.keys():
            rmList.append(item)
        for key in rmList:
            self.close(key)

    # list help information
    def help(self):
        print("Writen by Jing Lan, Jan. 26th 2021")
        print("Mind that for upload/download, you should specify both local and remote path")
        print("And r(ead)/w(rite) mode to open files\n")

        print("{:<10}|  {:<50}".format("Usage", "command paras"))
        for command in helpDict.keys():
            print("{:<10}|  {:<50}".format(command, helpDict[command]))

helpDict = {
    "pwd" : "current remote path",
    "ls" : "files/dirs under pwd",
    "cd" : "enter a given path",
    "mkdir" : "create a remote dir",
    "rm" : "remove a remote file. Recursive delete for folders",
    "upload" : "upload a local file",
    "download" : "download a remote file",
    "open" : "open a remote file, in r(ead)/w(rite) mode",
    "close" : "close with file id(lookup with lso), update changes(if any)",
    "lso" : "list files opened",
    "clear" : "clear the screen",
    "exit" : "exit the client and close all files"
}
        
def cmd(rootPath):
    print("This is a simplest distributed file system, writen by Jing Lan.")
    print("Enter 'help' to get supported operations")

    client = Client(rootPath)
    indent = "{}$ ".format(client.curPath)

    while True:
        print(indent, end='')
        command = input().split()
        op = command[0].lower()

        if op == "help":
            client.help()
        elif op == "pwd":
            client.pwd(1)
        elif op == "ls":
            client.ls()
        elif op == "cd":
            client.cd(command[1])
        elif op == "mkdir":
            client.mkdir(command[1])
        elif op == "rm":
            client.rm(command[1])
        elif op == "upload":
            if len(command) < 2:
                print("Missing parameters: upload local_file remote_path")
                continue

            client.upload(command[1], command[2], 0)
        elif op == "download":
            if len(command) < 2:
                print("Missing parameters: download remote_file local_path")
                continue

            client.download(command[2], command[1], 0)
        elif op == "open":
            if len(command) < 3:
                print("Missing parameters: open remote_file local_path open_mode")
                continue

            if command[3] == "r" or command[3] == "read":
                openFlag = 0
            elif command[3] == "w" or command[3] == "write":
                openFlag = 1
            else:
                print("Unknown open mode. Candidates: r(ead)/w(rite)")
                continue
            
            client.open(command[2], command[1], openFlag)
        elif op == "close":
            if len(command) < 1:
                print("Missing parameters: close file_id")
                continue

            client.close(command[1])
        elif op == "lso":
            client.listopen()
        elif op == "clear":
            os.system("clear")
        elif op == "exit":
            client.exit()
            return 

if __name__ == "__main__":
    cmd("/home/jing/code/dfs/data")
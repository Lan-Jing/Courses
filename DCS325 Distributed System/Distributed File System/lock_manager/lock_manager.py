import grpc
import lock_manager_pb2 as lm
import lock_manager_pb2_grpc as lm_grpc

from concurrent import futures

import lm_conf

class lock_manager(lm_grpc.lock_managerServicer):
    def __init__(self):
        self.ip = lm_conf.lockManagerIP
        self.port = lm_conf.lockManagerPort
        self.lockDict = {}
        print("The lock manager is available on {}:{}\n".format(self.ip, self.port))

    # supports both shared and exclusive lock.
    def lock(self, request, context):
        path = request.filePath  
        lockStatus = self.lockDict.get(path, 0)
        if lockStatus == -1 or (lockStatus > 0 and request.lockType == 1):
            print("Failed to lock on {}, status:{}".format(path, lockStatus))
            return lm.lm_reply(done=0)
        else:
            if request.lockType == 0:
                self.lockDict[path] = lockStatus + 1
            else:
                self.lockDict[path] = -1

            print("{} locked, status:{}".format(path, self.lockDict[path]))
            return lm.lm_reply(done=1)
        
        return lm.lm_reply(done=0)

    # pop out one of the locks on a given file
    def unlock(self, request, context):
        path = request.filePath
        lockStatus = self.lockDict.get(path, 0)
        if lockStatus > 0:
            self.lockDict[path] -= 1
        if lockStatus == 1 or lockStatus == -1:
            self.lockDict.pop(path, 0)

        print("{} unlocked".format(path))
        return lm.lm_reply(done=1)
    
    def print_locks(self):
        print("Files with Lock(s):")
        for key in self.lockDict.keys():
            print("file: {}, status:{}".format(key, self.lockDict[key]))

def serve():
    manager = lock_manager()
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    lm_grpc.add_lock_managerServicer_to_server(manager, server)
    server.add_insecure_port("[::]:{}".format(manager.port))
    server.start()
    server.wait_for_termination()

if __name__ == "__main__":
    serve()
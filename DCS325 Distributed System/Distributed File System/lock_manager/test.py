import grpc
import lock_manager_pb2 as lm
import lock_manager_pb2_grpc as lm_grpc
import conf as lm_conf

def lock(stub, path, t):
    reply = stub.lock(lm.fileInfo(filePath=path, lockType=t))
    if reply.done == True:
        print("done")
    else:
        print("failed")

def unlock(stub, path):
    reply = stub.unlock(lm.fileInfo(filePath=path))
    if reply.done == True:
        print("done")
    else:
        print("failed")

if __name__ == "__main__":
    print("Connecting to {}:{}".format(lm_conf.lockManagerIP, lm_conf.lockManagerPort))
    channel = grpc.insecure_channel(lm_conf.lockManagerIP + ":" + lm_conf.lockManagerPort)
    lmStub = lm_grpc.lock_managerStub(channel)

    path = "/text.txt"
    path1 = "/asd.exe"
    lock(lmStub, path, 0)
    lock(lmStub, path, 0)
    lock(lmStub, path, 0)
    lock(lmStub, path, 1)
    lock(lmStub, path1, 0)
    unlock(lmStub, path)
    unlock(lmStub, path)
    unlock(lmStub, path)
    lock(lmStub, path, 1)
    lock(lmStub, path, 0)
    unlock(lmStub, path)
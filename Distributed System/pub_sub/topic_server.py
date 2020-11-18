import grpc
import topic_pb2 as tp
import topic_pb2_grpc as tpg

from concurrent import futures
import threading
import time

# a timeout function that remove messages periodically
def thread_timeout(messageDir, dirLock):
    while True:
        time.sleep(1)
        dirLock.acquire()
        
        print("Server now holds {} messages.".format(len(messageDir.keys())))
        rmList = []
        for i in messageDir.keys():
            messageDir[i][0] -= 1
            if messageDir[i][0] == 0:
                rmList.append(i)
        for i in rmList:
            del messageDir[i]

        dirLock.release()

# implementation of the topic servicer
# able to control maximum connection
class topicServicer(tpg.topicServicer):
    def __init__(self, maxConnection=5):
        self.messageDir = {}
        self.subCnt = maxConnection

        self.dirLock = threading.Lock()
        self.conLock = threading.Lock()
        # spawn helper thread to maintain messages
        timeoutThread = threading.Thread(target=thread_timeout, args=(self.messageDir, self.dirLock))
        timeoutThread.start()

    def publish(self, request, context):
        self.dirLock.acquire()

        if request.category in self.messageDir:
            self.dirLock.release()
            return tp.ack(ack=0)
        self.messageDir[request.category] = [request.timeoutLimit, request.content]
        
        self.dirLock.release()
        return tp.ack(ack=1)

    def subscribe(self, request, context):
        self.conLock.acquire()
        
        # connection not available
        if self.subCnt == 0:
            self.conLock.release()
            return tp.messageList(ack=0)
        else:
            self.subCnt -= 1
        
        self.conLock.release()
        
        messageList = tp.messageList(ack=1)
        self.dirLock.acquire()
        
        for category in request.categories:
            # no message of specific kind
            if category not in self.messageDir:
                continue
            messageList.message.append(tp.subMessage(category=category, content=self.messageDir[category][1]))
        
        self.dirLock.release()
        return messageList

    def sub_ack(self, request, context):
        self.conLock.acquire()

        self.subCnt += request.ack
        
        self.conLock.release()
        return tp.empty()

# initialize the server
def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    tpg.add_topicServicer_to_server(topicServicer(), server)
    server.add_insecure_port('[::]:50051')
    server.start()
    server.wait_for_termination()

if __name__ == "__main__":
    serve()
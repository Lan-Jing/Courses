import grpc
import topic_pb2 as tp
import topic_pb2_grpc as tpg

import random
import time

# pull messages from the topic, then respond to release the connection
def subscribe(stub):
    # generate random request
    sList = tp.subscriberInfo()
    for i in range(3):
        sList.categories.append(random.randint(0, 5))
    
    messageList = stub.subscribe(sList)
    
    if messageList.ack == 0:
        print("Connection to the topic denied.\n")
    else:
        for message in messageList.message:
            print("Subscribed message: {}\n".format(message))
        sub_ack(stub)

# response to the topic
def sub_ack(stub):
    stub.sub_ack(tp.ack(ack=1))

# periodically pull message from the topic
def run():
    while True:
        time.sleep(1)
        with grpc.insecure_channel('localhost:50051') as channel:
            stub = tpg.topicStub(channel)
            subscribe(stub)

if __name__ == '__main__':
    run()
import grpc
import topic_pb2 as tp
import topic_pb2_grpc as tpg

import random
import time

# publish message to the topic
def publish(stub):
    category = random.randint(0, 5)
    content = "Message of category {}".format(category)
    ack = stub.publish(tp.pubMessage(category=category, timeoutLimit=3, content=content))
    if ack.ack == 1:
        print("{} is published.\n".format(content))
    else:
        print("{} failed to be published.\n".format(content))

# periodically publish messages to the topic
def run():
    while True:
        with grpc.insecure_channel('localhost:50051') as channel:
            stub = tpg.topicStub(channel)
            publish(stub)
        time.sleep(1)

if __name__ == '__main__':
    run()
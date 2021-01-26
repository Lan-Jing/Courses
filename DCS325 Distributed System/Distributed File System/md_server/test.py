import grpc
import md_server_pb2 as ms
import md_server_pb2_grpc as ms_grpc
import conf as ms_conf

import time

if __name__ == "__main__":
    channel = grpc.insecure_channel(ms_conf.md_serverIP + ":" + ms_conf.md_serverPort)
    mdStub = ms_grpc.md_serverStub(channel)

    newId = (mdStub.connect(ms.empty())).done
    print("ID {}".format(newId))
    res = mdStub.disconnect(ms.ms_reply(done=newId))
    print("reply {}".format(res.done))

    res = mdStub.online(ms.serverInfo(ip="1.1.1.1", port="1000", id=newId))
    res = mdStub.online(ms.serverInfo(ip="1.1.1.2", port="2000", id=newId+1))
    res = mdStub.online(ms.serverInfo(ip="1.1.1.2", port="3000", id=newId+2))
    slist = mdStub.listServer(ms.empty())
    for info in slist.sList:
        print(info.id, ":", info.ip, " ", info.port)

    time.sleep(2)

    res = mdStub.offline(ms.serverInfo(ip="1.1.1.1", port="1000", id=newId))
    res = mdStub.offline(ms.serverInfo(ip="1.1.1.2", port="2000", id=newId+1))

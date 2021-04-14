# The Simplest Distributed File System 

As the final project of DCS325 Distributed System

Jan. 24th, 2021 ~ Jan. 25th, 2021

## Environment

* Ubuntu 20 on WSL2
* Python 3.8.5
* Google Protobuf and grpc, installed with pip3

## Getting it

1. Download the whole folder and Python scripts inside. **Edit root paths for file servers and clients. You cans see them at the bottom.**
2. Run the metadata server. IP:port is configured in ms_conf.py

```bash
    Python3 md_server.py
```

3. Run the lock manager. 

```bash
    Python3 lock_manager.py
```

4. Run multiple file servers, then multiple clients. Specify ports for servers with an argument:

```bash
    Python3 file_server.py 8001
```

## Structure and Usage

### Structure and Workflow

![Struct](assets/struct.png?raw=true "Structure of the File System")


* The lock manager and the metadata server(MDS) should be first online.
* Then file servers should register themselves to the MDS, getting their unique IDs.
* File servers send heartbeat to the MDS in every circle(e.g. five seconds), or will be considered offline.
* Clients request list of active servers, then choose one to connect to.

### Features

* Unique IDs for both file servers and clients. Each client can hold its own remote working directory.
* Support shared(read)/exclusive(write) file access.
* Transaction-like atomicity. Broadcast updates to replicas within a remote operation.
* Simplest isolation. Clients no premitted to see and operate outside the root of servers.

### Usage

* help : output the following messages
* pwd : current remote working directory
* cd : enter a given path
* ls : list files/dirs under pwd
* mkdir : create a remote dir
* rm : remove a remote file. Recursive delete for folders.
* upload : upload local_file as remote_file
* download : download remote_file as local_file
* open : open a remote file, in r(ead)/w(rite) mode
* close : with file id(look up by lso), update changes if in write mode
* lso : list opened files
* clear : clear the screen
* exit : close all files and exit the client
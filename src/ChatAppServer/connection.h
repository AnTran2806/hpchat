#ifndef CONNECTION_H
#define CONNECTION_H

#include "../common/ChatAppServer/library.h"
#include "server.h"
#include "userAuthentication.h"

class Connection{
public:
    Connection(class UserAuthentication& auth);
    void start(int port);

private:
    int maxSocket;
    fd_set readfds;
    int serverSocket;
    int clientSocket;    
    sockaddr_in serverAddr;
    vector<int> clientSockets;
    class UserAuthentication& auth;
};

#endif // CONNECTION_H
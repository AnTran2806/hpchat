#ifndef CONNECTION_H
#define CONNECTION_H
#include "server.h"
using namespace std;

class Connection{
private:
    // class Server& server;
    class UserAuthentication& auth;
    int serverSocket;
    int clientSocket;    
    int maxSocket;
    sockaddr_in serverAddr;
    vector<int> clientSockets;
    fd_set readfds;

public:
    // Connection(class Server& server);
    Connection(class UserAuthentication& auth);
    void start(int port);
};

#endif // CONNECTION_H
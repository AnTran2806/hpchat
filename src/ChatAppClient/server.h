#ifndef SERVER_H
#define SERVER_H

#include "../common/ChatAppClient/library.h"
#include "client.h"

class Server {
public:
    int serverSock;
    int clientSock;
    thread serverThread;

    ~Server();

    void stopServer() ;

    bool becomeServer(int port,string clientName); 

    void handleP2PMessages(int clientSock,string clientName);   
};

#endif // SERVER_H
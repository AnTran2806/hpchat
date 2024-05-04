#ifndef CONNECTION_H
#define CONNECTION_H

#include "../common/ChatAppClient/library.h"

class Connection {
public:
    Connection(class Client& client);
    bool connectToServer(const string& ipAddress, int port);

private:
    int clientSocket;    
    class Client& client;
};

#endif // CONNECTION_H

#ifndef CONNECTION_H
#define CONNECTION_H

#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class Connection {
public:
    Connection(class Client& client);
    bool connectToServer(const std::string& ipAddress, int port);

private:
    int clientSocket;    
    class Client& client;
};

#endif // CONNECTION_H

#ifndef CLIENT_H
#define CLIENT_H

#include "../common/ChatAppClient/library.h"
#include "authentication.h"
#include "server.h"

const int PORT = 55000;

void sendToServer(int socket,const string& message);

class Server;

class Client {
private:
    Server* server;
    int clientSocket;
    thread receivingThread;
    
public:
    Client(Server* server);

    explicit Client(int socket);

    Client();
    ~Client();

    int getSocket() const;

    string getIPAddress();

    void processFunction();

    void sendFile(int clientSocket);

    void stopClient(int clientSocket);

    void receiveFile(int clientSocket);

    void startReceiving(int clientSocket);

    bool connectToServer(const string& ipAddress, int port);
};

#endif // CLIENT_H
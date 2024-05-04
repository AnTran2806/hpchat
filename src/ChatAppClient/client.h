#ifndef CLIENT_H
#define CLIENT_H

#include "../common/ChatAppClient/library.h"
#include "authentication.h"

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

class Server{
public:
    int serverSock;
    int clientSock;
    thread serverThread; 

    ~Server(){
        stopServer();
    }
    void stopServer() ;

    bool becomeServer(int port,string clientName); 

    void handleP2PMessages(int clientSock,string clientName);
};

#endif // CLIENT_H
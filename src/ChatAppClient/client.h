#ifndef CLIENT_H
#define CLIENT_H

#include "../common/ChatAppClient/library.h"
#include "authentication.h"

const int PORT = 55000;

void sendToServer(int socket,const string& message);

class Client {
private:
    int sock;
    thread receivingThread;

    
public:
    Client();

    Client(int sock):sock(sock){};

    ~Client(){
        if (receivingThread.joinable()) {
            receivingThread.join();
        }
    }

    int getSocket() const {
        return sock;
    }

    void stopClient(int clientSocket){
        close (clientSocket);
    }
    
    bool connectToServer(const string& ipAddress, int port);

    void startReceiving(int clientSocket);

    void processFunction();

    string getIPAddress();


};

class Server{
          
    public:
        int AserverSock;
        int AclientSock;
        thread AserverThread; 

        ~Server(){
            stopServer();
        }
        void stopServer() ;

        bool becomeServer(int port,string clientName); 

        void handleP2PMessages(int clientSock,string clientName);
};

class SharingFile{
public:
    void sendFile(int clientSocket);

    void receiveFile(int clientSocket);

};
#endif // CLIENT_H
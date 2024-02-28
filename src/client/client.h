#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <algorithm>
#include <cctype>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fstream>
#include <vector>
#include <ifaddrs.h>
#include <functional>

using namespace std;

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

class Authen {

public:
    void handleUserInteraction(int clientSocket);

    void enterCredential(int clientSocket);

};

class SharingFile{
public:
    void sendFile(int clientSocket);

    void receiveFile(int clientSocket);

};
#endif // CLIENT_H
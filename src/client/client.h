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


using namespace std;

const int PORT = 4444;
const string IP_SERVER = "10.188.9.20";

class Client {
private:
    int sock;
    thread receivingThread;

    
public:
    Client();
    ~Client();
    
    bool connectToServer(const string& ipAddress, int port);

    void startReceiving();
    void receiveFile();

    void sendToServer(const string& message);

    void handleUserInteraction();

    string trim(const string& str);
};

#endif // CLIENT_H
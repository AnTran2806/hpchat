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

class Client {
private:
    int createSocketClient;
    std::thread receivingThread;

    

public:
    Client();

    ~Client();

    std::string trim(const std::string& str);
    
    bool connectToServer(const std::string& ipAddress, int port);

    void runMessageReceiver();

    void sendToServer(const std::string& message);
};

#endif // CLIENT_H

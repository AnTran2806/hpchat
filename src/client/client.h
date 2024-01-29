#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fstream>
#include <chrono>
#include <ctime>

class Client {
private:
    int sock;
    std::thread receivingThread;
    std::string clientName;
    std::string roomName;
    
public:
    Client();
    ~Client();

    void startReceiving();
    void receiveFile();

    std::string trim(const std::string& str);
    bool connectToServer(const std::string& ipAddress, int port);
    void runMessageReceiver();
    void sendToServer(const std::string& message);
    void sendFile(const std::string& filePath);
};

#endif // CLIENT_H

#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cctype>
#include <chrono>
#include <fstream>

class Client {
public:
    Client(int socket, const std::string& name, const std::string& roomName);

    int getSocket() const;

    const std::string& getName() const;

    const std::string& getRoomName() const;

private:
    int socket;
    std::string name;
    std::string roomName;
};

class Server {
public:
    Server();

    Client* findClientByName(const std::string& name);

    void sendPrivateMessage(const std::string& senderName, const std::string& receiverName, const std::string& message);

    void start();
    std::string trim(const std::string& str);
    
    std::string receiveString(int clientSocket);

    void handlePrivateMessage(int clientSocket, 
                            const std::string& clientName, 
                            const std::string& roomName,
                            std::string receivedMessage);
    void handleServerChat(int clientSocket, const std::string& clientName);
    void handleFileTransfer(int clientSocket, const std::string& clientName);

    void handleGroupMessage(const std::string& clientName, 
                            const std::string& roomName, 
                            const std::string& receivedMessage,
                            int clientSocket);

    void handleClientOffline(int clientSocket, const std::string& clientName);

private:
    int createSocket;
    std::vector<Client> clients;
    std::mutex clientsMutex;


    void handleClient(int clientSocket, const std::string& clientName, const std::string& roomName);
};

#endif // SERVER_H

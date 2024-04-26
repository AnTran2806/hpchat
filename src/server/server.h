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
#include <map>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sstream>
#include <future>
#include <unordered_map>
#include <set>
#include "userAuthentication.h"

using namespace std;

const int PORT = 55000;
const int BUFFER_SIZE = 1024;
const int MAX_CLIENTS = 10;

class Client {
public:
    Client(int socket, const string& name, const string& roomName);

    int getSocket() const;

    const string& getName() const;

    const string& getRoomName() const;

private:
    int socket;
    string name;
    string roomName;
};

class Connection{
private:
    // class Server& server;
    class UserAuthentication& auth;
    int serverSocket;
    int clientSocket;    
    int maxSocket;
    sockaddr_in serverAddr;
    vector<int> clientSockets;
    fd_set readfds;

public:
    // Connection(class Server& server);
    Connection(class UserAuthentication& auth);
    void start(int port);
};

class Server {
public:
    Server();

    Client* findClientByName(const string& name);

    void sendPrivateMessage(const string& senderName, const string& receiverName, const string& message);

    string trim(const string& str);

    string receiveString(int clientSocket);

    void handlePrivateMessage(int clientSocket, const string& clientName, const string& roomName, string receivedMessage);

    void handleServerChat(int clientSocket, const string &clientName);

    void handleFileTransfer(int clientSocket, string clientName,string roomID);

    void handleGroupMessage(const string& clientName, int clientSocket);

    void handleClientOffline(int clientSocket, const string &clientName);

    void processClient(int clientSocket);
    
    void handleClient(int clientSocket, const string& clientName, const string& roomName);

    void setLoggedInUsers(unordered_map<int, string> loggedInUsers){
        this->loggedInUsers = loggedInUsers;
    }

private:
    int PORT;
    vector<Client> clients;
    mutex clientsMutex;
    unordered_map<int, string> loggedInUsers;
    UserAuthentication auth;
    vector<pair<int,string>> checkRoomIDs;
};

#endif // SERVER_H
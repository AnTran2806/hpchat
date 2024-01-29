#ifndef SERVER_H
#define SERVER_H

#include <iostream>
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

using namespace std;

const int PORT = 4444;
const int BUFFER_SIZE = 1024;
const int MAX_CLIENTS = 10;

class UserAuthentication
{
private:
    string username;
    string password;


public:
    UserAuthentication();

    bool isUserRegistered(const string &checkUsername);

    bool registerUser(const string &enteredUsername, const string &enteredPassword);

    bool isLoggedIn(const string &enteredUsername, const string &enteredPassword);
};

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

class Server {
public:
    Server();

    Client* findClientByName(const string& name);

    void sendPrivateMessage(const string& senderName, const string& receiverName, const string& message);

    void start(int port);

    void printColoredIP(const char* ipAddress);

    string trim(const string& str);

    string receiveString(int clientSocket);

    void handlePrivateMessage(int clientSocket, const string& clientName, const string& roomName, string receivedMessage);

    void handleServerChat(int clientSocket, const string &clientName);

    void handleFileTransfer(int clientSocket, const string& clientName);

    void handleGroupMessage(const string& clientName, const string& roomName, const string& receivedMessage, int clientSocket);

    void handleClientOffline(int clientSocket, const string &clientName);

private:
    int serverSocket;
    int clientSocket;
    int maxSocket;
    sockaddr_in serverAddr;
    vector<Client> clients;
    mutex clientsMutex;
    UserAuthentication auth;
    vector<int> clientSockets;
    fd_set readfds;

    unordered_map<int, string> loggedInUsers;  // Used to store logged in user names


    bool handleRegistration(int clientSocket);
    bool handleLogin(int clientSocket);
    void handleAuthentication(int clientSocket, const string& option);

    void processClient(int clientSocket);
    void handleClient(int clientSocket, const string& clientName, const string& roomName);
};

#endif // SERVER_H
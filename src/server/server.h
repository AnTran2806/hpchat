#ifndef SERVER_H
#define SERVER_H

// class Connection;

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

using namespace std;

const int PORT = 55000;
const int BUFFER_SIZE = 1024;
const int MAX_CLIENTS = 10;

class UserAuthentication
{
private:
    class Server* server;
    string username;
    string password;

public:
    UserAuthentication();  
    UserAuthentication(class Server* server);

    bool isUserRegistered(const string &checkUsername);

    bool registerUser(const string &enteredUsername, const string &enteredPassword);

    bool isLoggedIn(const string &enteredUsername, const string &enteredPassword);

    bool changePassword(const string& enteredUsername, const string& oldPassword, const string& newPassword);

    bool deleteAccount(const string& enteredUsername, const string& retypePassword);
    
    void handleAuthentication(int clientSocket, const string& option);

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

    bool handleRegistration(int clientSocket);
    bool handleLogin(int clientSocket);
    bool handleChangePassword(int clientSocket);
    bool handleDeleteAccount(int clientSocket);

    void processClient(int clientSocket);
    void handleClient(int clientSocket, const string& clientName, const string& roomName);
private:
    int PORT;
    vector<Client> clients;
    mutex clientsMutex;
    UserAuthentication auth;
    unordered_map<int, string> loggedInUsers;  // Used to store logged in user names
    vector<pair<int,string>> checkRoomIDs;


};

#endif // SERVER_H
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
using namespace std;

using namespace std;

using namespace std;

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

    void handlePrivateMessage(const string& senderName, const string& receiverName, const string& message);

    void start();

private:
    int serverSocket;
    vector<Client> clients;
    mutex clientsMutex;
    UserAuthentication auth;

    bool handleRegistration(int clientSocket);
    bool handleLogin(int clientSocket);

    string trim(const string& str);
    string receiveString(int clientSocket);
    void handleClient(int clientSocket, const string& clientName, const string& roomName);
    void handleClient(int clientSocket, const string& clientName, const string& roomName);
    string handleAuthentication(int clientSocket);
};

#endif // SERVER_H

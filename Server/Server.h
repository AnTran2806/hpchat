#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>
#include <thread>
#include <map>
#include <fstream>
#include <mutex>
#include <vector>
#include <algorithm>

using namespace std;

const int PORT = 4444;
const int BUFFER_SIZE = 1024;
const int MAX_CLIENTS = 10;

class UserAuthentication {
private:
    std::string username;
    std::string password;

public:
    UserAuthentication();
    bool isUserRegistered(const std::string& checkUsername);
    bool registerUser(const std::string& enteredUsername, const std::string& enteredPassword);
    bool isLoggedIn(const std::string& enteredUsername, const std::string& enteredPassword);
};

class TCPServer {
private:
    int serverSocket;
    int maxSocket;
    sockaddr_in serverAddr;
    vector<int> clientSockets;
    fd_set readfds;
    mutex clientsMutex;
    UserAuthentication auth;

    void closeConnection();
    

// private:
    // void handleRegistration(int clientSocket);
    // void handleLogin(int clientSocket);
    // void processCase();
    // void receiveData();
    // void sendData(const std::string& data);
    // void handleClient(int clientSocket);
    // void closeConnection();
    // void displayClientInfo(const sockaddr_in& client);
    

public:
    bool start(int port){};

};

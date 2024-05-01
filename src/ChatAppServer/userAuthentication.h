#ifndef USERAUTHENTICATION_H
#define USERAUTHENTICATION_H

// #include <string>
// #include <unordered_map>
// using namespace std;
#include "../common/library.h"

class Server;

class UserAuthentication {
private:
    Server* server;
    string username;
    string password;

public:
    UserAuthentication();  
    UserAuthentication(Server* server);

    unordered_map<int, string> loggedInUsers;  // Used to store logged in user names

    bool isUserRegistered(const string &checkUsername);

    bool registerUser(const string &enteredUsername, const string &enteredPassword);

    bool isLoggedIn(const string &enteredUsername, const string &enteredPassword);

    bool changePassword(const string& enteredUsername, const string& oldPassword, const string& newPassword);

    bool deleteAccount(const string& enteredUsername, const string& retypePassword);
    
    void handleAuthentication(int clientSocket, const string& option);

    bool handleRegistration(int clientSocket);

    bool handleLogin(int clientSocket, Server* server);

    bool handleChangePassword(int clientSocket);

    bool handleDeleteAccount(int clientSocket);
};

#endif // USERAUTHENTICATION_H
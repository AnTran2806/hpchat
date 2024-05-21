#ifndef USERAUTHENTICATION_H
#define USERAUTHENTICATION_H

#include "../common/ChatAppServer/library.h"

class Server;

class UserAuthentication {
public:
    UserAuthentication(); 

    UserAuthentication(Server* server);

    bool handleRegistration(int clientSocket);
    
    bool handleDeleteAccount(int clientSocket);

    bool handleChangePassword(int clientSocket);

    bool handleLogin(int clientSocket, Server* server);

    bool isUserRegistered(const string &checkUsername);

    void handleAuthentication(int clientSocket, const string& option);

    bool isLoggedIn(const string &enteredUsername, const string &enteredPassword);
    
    bool registerUser(const string &enteredUsername, const string &enteredPassword);

    bool deleteAccount(const string& enteredUsername, const string& retypePassword);

    bool changePassword(const string& enteredUsername, const string& oldPassword, const string& newPassword, const string& confirmNewPassword);

private:
    Server* server;
    string username;
    string password;
    unordered_map<int, string> loggedInUsers;  // Used to store logged in user names
    unordered_map<int, string> loggedInPasswords;  // Used to store logged in user names
};

#endif // USERAUTHENTICATION_H
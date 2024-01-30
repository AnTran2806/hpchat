#include "server.h"

bool Server::handleRegistration(int clientSocket)
{
    char usernameBuffer[1024] = {0};
    char passwordBuffer[1024] = {0};

    recv(clientSocket, usernameBuffer, sizeof(usernameBuffer), 0);
    recv(clientSocket, passwordBuffer, sizeof(passwordBuffer), 0);

    string username(usernameBuffer);
    string password(passwordBuffer);

    bool status = auth.isUserRegistered(username);
    status = !status && auth.registerUser(username, password);
    const char *response;
    if (status)
    {
        response = "Registration successful.";
    }
    else
    {
        response = "Username already exists. Choose a different username.";
    }
    send(clientSocket, response, strlen(response), 0);
    return status;
}
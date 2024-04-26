#include "server.h"

bool UserAuthentication::handleRegistration(int clientSocket)
{
    char usernameBuffer[1024] = {0};
    char passwordBuffer[1024] = {0};

    recv(clientSocket, usernameBuffer, sizeof(usernameBuffer), 0);
    recv(clientSocket, passwordBuffer, sizeof(passwordBuffer), 0);

    string username(usernameBuffer);
    string password(passwordBuffer);

    bool status = isUserRegistered(username);
    status = !status && registerUser(username, password);
    const char *response;
    if (status)
    {
        response = "Registration successful. Please log in with your new credentials.";
        send(clientSocket, response, strlen(response), 0);
        handleLogin(clientSocket, server);
    }
    else
    {
        response = "Username already exists. Choose a different username.\nPlease try again.";
        send(clientSocket, response, strlen(response), 0);

    }
    // send(clientSocket, response, strlen(response), 0);
    return status;
}
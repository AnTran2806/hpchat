#include "credentialHandler.h"

void CredentialHandler::receiveCredential(int clientSocket, string& username, string& password) {
    char usernameBuffer[1024] = {0};
    char passwordBuffer[1024] = {0};

    recv(clientSocket, usernameBuffer, sizeof(usernameBuffer), 0);
    recv(clientSocket, passwordBuffer, sizeof(passwordBuffer), 0);

    username = string(usernameBuffer);
    password = string(passwordBuffer);
}

void CredentialHandler::sendResponse(int clientSocket, const string &response) {
    send(clientSocket, response.c_str(), response.length(), 0);
}

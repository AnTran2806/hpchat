#include "server.h"

void Server::handleClient(int clientSocket, const std::string &clientName, const std::string &roomName)
{
    char buffer[4096];
    int bytesReceived;
    while (true)
    {
        // Receive data from the client
        bytesReceived = recv(clientSocket, buffer, 4096, 0);

        if (bytesReceived <= 0) {
            handleClientOffline(clientSocket, clientName);
            return;
        }
        std::string receivedMessage(buffer, 0, bytesReceived);
        // Handle private messages if the message starts with "/pm"
        if (receivedMessage.size() > 3 && receivedMessage.substr(0, 3) == "/pm")
        {
            handlePrivateMessage(clientSocket, clientName, roomName, receivedMessage);
        }
        // Handle server commands if the message starts with "/sv"
        else if (receivedMessage.size() > 3 && receivedMessage.substr(0, 3) == "/sv")
        {
            handleServerChat(clientSocket, clientName);
        }
        else if (receivedMessage == "sendfile") 
        {
            handleFileTransfer(clientSocket, clientName);
        }
        // Handle group messages if none of the above conditions are met
        else {
            handleGroupMessage(clientName, roomName, receivedMessage, clientSocket);
        }
    }
}
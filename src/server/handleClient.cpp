#include "server.h"

void Server::handleClient(int clientSocket, const string& clientName, const string& roomName) 
{
    // ... (handleClient implementation)
    char buffer[4096];
    int bytesReceived;
    while (true) 
    {
        bytesReceived = recv(clientSocket, buffer, 4096, 0);

        if (bytesReceived <= 0) {
            handleClientOffline(clientSocket, clientName);
            return;
        }

        string receivedMessage(buffer, 0, bytesReceived);
        // Check the user that want to send the privare Message
        if (receivedMessage.size() > 3 && receivedMessage.substr(0, 3) == "/pm") 
        {
            handlePrivateMessage(clientSocket, clientName, roomName, receivedMessage);

        } 

        else if (receivedMessage.size() > 3 && receivedMessage.substr(0, 3) == "/sv")
        {
            handleServerChat(clientSocket, clientName);

        }
            
        else if(receivedMessage.substr(0, 9) == "sendfile") {
            handleFileTransfer(clientSocket, clientName);
        }
        
        else 
        {
            handleGroupMessage(clientName, roomName, receivedMessage, clientSocket);
        }
    }
}
  

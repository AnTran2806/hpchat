#include "server.h"

void Server::handleClient(int clientSocket, const std::string& clientName, const std::string& roomName) {
        char buffer[4096];
        int bytesReceived;
        while (true) {
            bytesReceived = recv(clientSocket, buffer, 4096, 0);
            if (bytesReceived <= 0) {
                handleClientOffline(clientSocket, clientName);
                return;
            }
                //The fist message to receive after recieve the RoomName and The User Name
            std::string receivedMessage(buffer, 0, bytesReceived);
             // Handle Chat
            if (receivedMessage.size() > 3 && receivedMessage.substr(0, 3) == "/pm") {
                handlePrivateMessage( clientSocket,  clientName, roomName, receivedMessage );
            } else if (receivedMessage.size() > 3 && receivedMessage.substr(0, 3) == "/sv") {
                handleServerChat(clientSocket, clientName);
            }else if(receivedMessage=="sendfile") {
                handleFileTransfer(clientSocket, clientName);
            //////sendfile
            }else {
                handleGroupMessage(clientName, roomName, receivedMessage, clientSocket);
        }
    }
  
} 
#include "server.h"

void Server::handleGroupMessage(const string &clientName, 
                                const string &roomName, 
                                const string &receivedMessage, 
                                int clientSocket)
{
    string groupMessage = "\033[1;34m" + clientName + "\033[0m" + ": " + receivedMessage;
    // Lock the clientsMutex to ensure thread safety
    lock_guard<mutex> guard(clientsMutex);

    for (const auto &client : clients) {
        // Check if the client is in the same room and is not the sender
        if (client.getRoomName() == roomName && client.getSocket() != clientSocket)
        {
            string roomMessage = "\033[1;35m" + client.getRoomName() + "\033[0m" + ">>" + groupMessage;
            // Send the room message to the client's socket
            send(client.getSocket(), roomMessage.c_str(), roomMessage.size() + 1, 0);
        }
    }
}

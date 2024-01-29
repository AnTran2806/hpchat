#include "server.h"

void Server::handleGroupMessage(const std::string &clientName, 
                                const std::string &roomName, 
                                const std::string &receivedMessage, 
                                int clientSocket)
{
    std::string groupMessage = "\033[1;34m" + clientName + "\033[0m" + ": " + receivedMessage;
    // Lock the clientsMutex to ensure thread safety
    std::lock_guard<std::mutex> guard(clientsMutex);

    for (const auto &client : clients) {
        // Check if the client is in the same room and is not the sender
        if (client.getRoomName() == roomName && client.getSocket() != clientSocket)
        {
            std::string roomMessage = "\033[1;35m" + client.getRoomName() + "\033[0m" + ">>" + groupMessage;
            // Send the room message to the client's socket
            send(client.getSocket(), roomMessage.c_str(), roomMessage.size() + 1, 0);
        }
    }
}

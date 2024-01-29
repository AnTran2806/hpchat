#include "server.h"

void Server::sendPrivateMessage(const std::string &senderName, const std::string &receiverName, const std::string &message)
{
    // Lock the clientsMutex to ensure thread safety
    std::lock_guard<std::mutex> guard(clientsMutex);

    // Find the sender in the clients array
    Client *sender = nullptr;
    for (auto &client : clients)
    {
        if (client.getName() == senderName) {
            sender = &client;
        }
    }

    // If the sender is found, iterate through all clients to find the receiver
    if (sender) {
        for (auto &client : clients)
        {
            if (client.getName() == receiverName)
            {
                send(client.getSocket(), message.c_str(), message.size() + 1, 0);
                return;
            }
        }
        std::cerr << "Error: Receiver not found!" << std::endl;
    }
    else {
        std::cerr << "Error: Sender not found!" << std::endl;
    }
}
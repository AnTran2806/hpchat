#include "server.h"

void Server::sendPrivateMessage(const std::string &senderName, const std::string &receiverName, const std::string &message) {
    std::lock_guard<std::mutex> guard(clientsMutex);
    Client *sender = nullptr;
    for (auto &client : clients)
    {
        if (client.getName() == senderName)
        {
            sender = &client;
        }
    }

    // If the sender is found, iterate through all clients to find the receiver
    if (sender)
    {
        for (auto &client : clients)
        {
            if (client.getName() == receiverName)
            {
                send(client.getSocket(), message.c_str(), message.size() + 1, 0);
                // std::cout << "Send result: " << sendResult << std::endl;
                //  Print the private message sent from sender to receiver
                //  std::cout << "Private message sent from " << senderName << " to " << receiverName << ": " << message << std::endl;
                return; 
            }
        }
        // Print an error message if the receiver is not found
        std::cerr << "Error: Receiver not found!" << std::endl;
    }
    else
    {
        // Print an error message if the sender is not found
        std::cerr << "Error: Sender not found!" << std::endl;
    }
}
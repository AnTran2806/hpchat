#include "chatService.h"

void ChatService::sendPrivateMessage(const string &senderName, const string &receiverName, const string &message)
{
    // Lock the clientsMutex to ensure thread safety
    lock_guard<mutex> guard(clientsMutex);

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
        cerr << "Error: Receiver not found!" << endl;
    }
    else {
        cerr << "Error: Sender not found!" << endl;
    }
}
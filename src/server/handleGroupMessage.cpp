#include "server.h"

void Server::handleGroupMessage(const std::string& clientName, const std::string& roomName, const std::string& receivedMessage,int clientSocket) {
    std::string groupMessage = "\033[1;34m" + clientName + "\033[0m" + ": " + receivedMessage;
    std::lock_guard<std::mutex> guard(clientsMutex);
    
    for (const auto& client : clients) {
        if (client.getRoomName() == roomName && client.getSocket() != clientSocket) {
            std::string tenPhong = "\033[1;35m" + client.getRoomName() + "\033[0m" + ">>" + groupMessage;
            send(client.getSocket(), tenPhong.c_str(), tenPhong.size() + 1, 0);
        }
    }
}
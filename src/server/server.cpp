#include "server.h"

Client::Client(int socket, const std::string &name, const std::string &roomName) : socket(socket), name(name), roomName(roomName) {}

int Client::getSocket() const {
    return socket;
}

const std::string &Client::getName() const {
    return name;
}

const std::string &Client::getRoomName() const {
    return roomName;
}

std::string Server::trim(const std::string &str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");

    if (first == std::string::npos || last == std::string::npos)
        return "";
    else
        return str.substr(first, last - first + 1);
}

std::string Server::receiveString(int acceptConnection) {
    char buffer[4096];
    int bytesReceived = recv(acceptConnection, buffer, 4096, 0); 
    if (bytesReceived <= 0)
    {
        std::cerr << "Error: Received data from client failed!" << std::endl;
        return "";
    }
    return std::string(buffer, 0, bytesReceived); 
}

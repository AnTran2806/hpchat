#include "client.h"

Client::Client() {
    createSocketClient = socket(AF_INET, SOCK_STREAM, 0);
    if (createSocketClient == -1) {
        std::cerr << "Error: Create the socket" << std::endl;
        return;
    }
}

Client::~Client() {
    close(createSocketClient);
    if (receivingThread.joinable()) {
        receivingThread.join();
    }
}

void Client::sendToServer(const std::string& message) {
    send(createSocketClient, message.c_str(), message.size() + 1, 0);
}

std::string Client::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");

    if (first == std::string::npos || last == std::string::npos)
        return "";
    else
        return str.substr(first, last - first + 1);
}

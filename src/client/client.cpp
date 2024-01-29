#include "client.h"

Client::Client() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Error: Create the socket" << std::endl;
        return;
    }
}

Client::~Client() {
    close(sock);
    if (receivingThread.joinable()) {
        receivingThread.join();
    }
}

void Client::sendToServer(const std::string& message) {
    send(sock, message.c_str(), message.size() + 1, 0);
}

std::string Client::trim(const std::string& str) {
    // Find the index of the first and last characters in the string that is not whitespace or newline
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    // Check if either index is not found (indicating an empty string or a string with only whitespace)
    if (first == std::string::npos || last == std::string::npos)
        return "";
    else
        return str.substr(first, last - first + 1);
}

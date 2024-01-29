#include "client.h"

Client::Client() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        cerr << "Error: Create the socket" << endl;
        return;
    }
}

Client::~Client() {
    close(sock);
    if (receivingThread.joinable()) {
        receivingThread.join();
    }
}

void Client::sendToServer(const string& message) {
    send(sock, message.c_str(), message.size() + 1, 0);
}


string Client::trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");

    if (first == string::npos || last == string::npos)
        return "";
    else
        return str.substr(first, last - first + 1);
}
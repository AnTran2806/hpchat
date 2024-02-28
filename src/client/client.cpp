#include "client.h"

Client::Client() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        cerr << "Error: Create the socket" << endl;
        return;
    }
}

void sendToServer(int socket,const string& message) {
    send(socket, message.c_str(), message.size() + 1, 0);
}
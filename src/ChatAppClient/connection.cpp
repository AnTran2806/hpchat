#include "connection.h"
#include "client.h"

Connection::Connection(class Client& client) : client(client) {}

bool Connection::connectToServer(const string& ipAddress, int port) {
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    int connectRes = connect(client.getSocket(), (sockaddr*)&hint, sizeof(hint));
    if (connectRes == -1) {
        cerr << "Error: Cannot connect to Server!" << endl;
        return false;
    }

    return true;
}
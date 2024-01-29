#include "client.h"

bool Client::connectToServer(const std::string& ipAddress, int port) {
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    // Attempt to connect to the server using the created socket and hint structure
    int connectRes = connect(clientSocket, (sockaddr*)&hint, sizeof(hint));
    if (connectRes == -1) {
        std::cerr << "Error: Cannot connect to Server!" << std::endl;
        return false;
    }

    return true;
}
#include "client.h"

bool Client::connectToServer(const std::string& ipAddress, int port) {
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    int connectRes = connect(createSocketClient, (sockaddr*)&hint, sizeof(hint));
    if (connectRes == -1) {
        std::cerr << "Error: Cannot connect to Server!" << std::endl;
        return false;
    }

    return true;
}
#include "client.h"

void Client::runMessageReceiver() {
    receivingThread = std::thread([this]() {
        char buffer[4096];
        while (true) {
            int bytesReceived = recv(createSocketClient, buffer, 4096, 0);
            if (bytesReceived <= 0) {
                std::cerr << "Error: Server is down" << std::endl;
                close(createSocketClient);
                break;
            }
            std::cout << std::string(buffer, 0, bytesReceived) << std::endl;
        }
    });
}
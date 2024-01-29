#include "client.h"    
    
void Client::startReceiving() {
    // Create a new thread to handle receiving messages from the server
    receivingThread = std::thread([this]() {
        char buffer[4096];
        // Continuously receive messages from the server
        while (true) {
            int bytesReceived = recv(sock, buffer, 4096, 0);
            if (bytesReceived <= 0) {
                std::cerr << "Error: Server is down" << std::endl;
                close(sock);
                break;
            }
            // Check if the received message indicates a file is being sent
            if (strcmp(buffer, "sendfile") == 0) {
                std::cout<<"Receiving a file from another client!";
                receiveFile();
            } else {
                std::cout << std::string(buffer, 0, bytesReceived) << std::endl;
            }
        }
    });
}
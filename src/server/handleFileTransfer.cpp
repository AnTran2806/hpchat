#include "server.h"

void Server::handleFileTransfer(int clientSocket, const std::string& clientName) {
    char buffer[4096];
    int bytesReceived;

    // Receive the file name from the client
    bytesReceived = recv(clientSocket, buffer, 4096, 0);
    if (bytesReceived <= 0) {
        std::cerr << "Error receiving file name from client " << clientName << std::endl;
        return;
    }
    std::string filename = std::string(buffer, 0, bytesReceived);

    // Open the file for writing
    std::ofstream fileDuc(filename, std::ios::binary | std::ios::app);
    if (!fileDuc.is_open()) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    std::cout << "Receiving file..." << std::endl;

    // Receive and write file data
    while (true) {
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesReceived <= 0) {
            // Exit the loop when receiving "endsendfile" or if there's an error
            break;
        }

        fileDuc.write(buffer, bytesReceived);
        std::cout << std::string(buffer, 0, bytesReceived);

        if (std::string(buffer, 0, bytesReceived) == "endsendfile") {
            // Exit the loop when receiving "endsendfile"
            break;
        }
    }

    if (bytesReceived < 0) {
        std::cerr << "Read Error" << std::endl;
    }

    // Close the file
    fileDuc.close();
    std::cout << "File received successfully: " << filename << std::endl;
}
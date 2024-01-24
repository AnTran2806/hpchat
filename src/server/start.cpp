#include "server.h"

void Server::start() {
    while (true)
    {
        sockaddr_in clientAddr;                                                            
        socklen_t clientSize = sizeof(clientAddr);                                         
        int acceptConnection = accept(createSocket, (sockaddr *)&clientAddr, &clientSize); 
        if (acceptConnection == -1) {
            std::cerr << "Error when accepting client connection: " << strerror(errno) << std::endl;
            continue;
        }

        // Receive room name from client
        std::string roomName = receiveString(acceptConnection);
        if (roomName.empty()) {
            close(acceptConnection);
            continue;
        }

        // Receive client name from client
        std::string clientName = receiveString(acceptConnection);
        if (clientName.empty()) {
            close(acceptConnection);
            continue;
        }

        std::lock_guard<std::mutex> guard(clientsMutex);
        clients.push_back(Client(acceptConnection, clientName, roomName));
        std::thread clientThread(&Server::handleClient, this, acceptConnection, clientName, roomName); 
        clientThread.detach();
    }
}
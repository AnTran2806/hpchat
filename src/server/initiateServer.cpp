#include "server.h"

void Server::initiateServer() {
    while (true)
    {
        // Accept new connection from client
        sockaddr_in clientAddr;
        socklen_t clientSize = sizeof(clientAddr);
        int acceptConnection = accept(serverSocket, (sockaddr *)&clientAddr, &clientSize);
        if (acceptConnection == -1)
        {
            std::cerr << "Error when accepting client connection: " << strerror(errno) << std::endl;
            continue;
        }

        std::string roomName = receiveString(acceptConnection);
        if (roomName.empty())
        {
            close(acceptConnection);
            continue;
        }

        std::string clientName = receiveString(acceptConnection);
        if (clientName.empty())
        {
            close(acceptConnection);
            continue;
        }

        // Add new client to clients vector
        std::lock_guard<std::mutex> guard(clientsMutex);
        clients.push_back(Client(acceptConnection, clientName, roomName));

        // Create new thread to handle client connection
        std::thread clientThread(&Server::handleClient, this, acceptConnection, clientName, roomName);
        clientThread.detach();
    }
}

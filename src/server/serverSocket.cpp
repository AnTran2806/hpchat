#include "server.h"

Server::Server() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        std::cerr << "Can't create the socket" << std::endl;
        return;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(8081);
    hint.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr *)&hint, sizeof(hint)) == -1)
    {
        std::cerr << "Bind failed!" << std::endl;
        close(serverSocket);
        return;
    }

    if (listen(serverSocket, SOMAXCONN) == -1)
    {
        std::cerr << "Listen failed!" << std::endl;
        close(serverSocket);
        return;
    }
}
#include "server.h"

Server::Server() {
    createSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (createSocket == -1)
    {
        std::cerr << "Can't create the socket" << std::endl;
        return;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(8080);
    hint.sin_addr.s_addr = INADDR_ANY;

    if (bind(createSocket, (sockaddr *)&hint, sizeof(hint)) == -1)
    {
        std::cerr << "Bind failed!" << std::endl;
        close(createSocket);
        return;
    }

    if (listen(createSocket, SOMAXCONN) == -1)
    {
        std::cerr << "Listen failed!" << std::endl;
        close(createSocket);
        return;
    }
}
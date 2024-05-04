#include "client.h"

void Server::stopServer() {
    close(serverSock);
    close(clientSock);

    if (serverThread.joinable()) {
        serverThread.join();
    }   
}